/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2020 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxCVoronoiFracture.h"
#include "DfxNvBlastGlobal.h"
#include "DfxFluentUtil.h"
#include "DfxOpenGLCacheQueue.h"


namespace compute_node {

	void VoronoiFractureNodeHandler::voronoiFractureMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

		auto _anyError = new fl::Any();
		auto error = new fl::InterpreterError;
		_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
		returnValues.push_back(_anyError);

		error->hasError = false;

		if (!args.empty()) {

			if (args[0]->type() != fl::type::FLOAT) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 0 ], expected float";
				return;
			}

			if (args[1]->type() != fl::type::FLOAT ||
				args[2]->type() != fl::type::FLOAT ||
				args[3]->type() != fl::type::FLOAT || 
				args[4]->type() != fl::type::FLOAT ) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 0, 1, 2, 3 ], expected float";
				return;
			}

			if (args[5]->type() != fl::type::STD_STRING) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 4 ], expected string";
				return;
			}

			if (args[6]->type() != fl::type::UNSIGNED_INT) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 5 ], expected uint";
				return;
			}

			if (args[7]->type() != fl::type::FLOAT_3) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 6 ], expected float";
				return;
			}

			if (args[8]->type() != fl::type::BOOL ||
				args[9]->type() != fl::type::BOOL ) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 7, 8 ], expected bool";
				return;
			}

			if (!args[7]->isArray()) {
				error->hasError = true;
				error->errorMsg = "Unknown Input Type [ 7 ], expected array ";
				return;
			}

			if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() ||
				args[3]->isArray() || args[4]->isArray() || args[5]->isArray() ||
				args[6]->isArray() || args[8]->isArray() || args[9]->isArray()) {
				error->hasError = true;
				error->errorMsg = "None of the inputs accept arrays.";
				return;
			}

			int materialID = std::max(0, (int)args[0]->data<float>());
			int seed  = std::max(1, (int)args[1]->data<float>());
			int cells = std::max(2, (int)args[2]->data<float>());
			int sites_per_cluster = std::max(2, (int)args[3]->data<float>());
			float cluster_radius  = std::max(0.001f, args[4]->data<float>());
			auto origin_s = args[5]->data<std::string>();
			auto output  = args[6]->data<unsigned int>();
			auto samples = args[7]->rawData<std::vector<glm::vec3>>();
			auto generate_clusters  = args[8]->data<bool>();
			auto inherit_attributes = args[9]->data<bool>();

			dfxblast::VoronoiOptions opts;
			opts.cell_count = cells;
			opts.rand_seed = seed;

			auto softResultset = DfxFluentSetManager::getInstance()->createSoftSet(DfxFluentSet::SIMPLEX);
			std::string destName = softResultset.first;
			if (!origin_s.empty()) {

				auto srcSet = DfxFluentSetManager::getInstance()->getSet(origin_s);

				if (srcSet != nullptr) {
					std::vector<size_t> originalSetIndices(srcSet->indices());
					std::vector<size_t> newIndices;

					if (srcSet->type() != DfxFluentSet::SIMPLEX) {
						error->hasError = true;
						error->errorMsg = "Only Simplex Sets Allowed For Source.";
						return;
					}
					
					std::vector<std::vector<Mesh*>> fragments_arr;
					fragments_arr.resize(srcSet->indices().size());

					auto xformAttrs = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform");
					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long long int i = 0; i < srcSet->indices().size(); i++) {
							try {

								auto mesh = DfxFluentSimplexCollection::getInstance()->simplexObjects()[srcSet->indices()[i]];
								auto xform = xformAttrs[srcSet->indices()[i]];
								auto fragments = dfxblast::DfxNvBlastGlobal::getInstance()->voronoiFracture(mesh->getGeometryAdapter(), 
									opts, xform, (*samples), sites_per_cluster, cluster_radius, generate_clusters, materialID);

								if (!fragments.empty()) {
									fragments_arr[i] = fragments;
								}					
							}
							catch (DfxNvBlastException const &e) {
								error->hasError = true;
								error->errorMsg = "Voronoi Fracture Failed to Fracture Simplex.";
							}
						}
					}

					size_t counter = 0;
					for (auto fragments : fragments_arr) {
						std::vector<size_t> simplexIndices;

						for (auto mesh : fragments) {

							auto obj = new DfxFluentSimplexObject;
							obj->copyMesh(mesh);
							auto new_xform = obj->moveToOrigin();
							DfxOpenGLCacheQueue::getInstance()->queueCache(obj->cache());
					
							auto index = DfxFluentSimplexCollection::getInstance()->addObject(obj, origin_s, "", new_xform);
							srcSet->addIndices(std::vector<size_t>{index});
							newIndices.push_back(index);
							simplexIndices.push_back(index);

							mesh->clear();
							delete mesh;
						}

						util::copyAttributes(DfxFluentSimplexCollection::getInstance()->atrributeMap(), srcSet->indices()[counter], simplexIndices);

						counter++;
					}

					if (output == 0) {
						for (size_t inx = 0; inx < originalSetIndices.size(); inx++) softResultset.second->indices().push_back(originalSetIndices[inx]);
					}
					else {
						for (size_t inx = 0; inx < newIndices.size(); inx++) softResultset.second->indices().push_back(newIndices[inx]);
					}

				}
				else {
					error->hasError = true;
					error->errorMsg = "Unknown origin set";
					return;
				}
			}

			auto rany = new fl::Any();
			rany->newData<std::string>(fl::type::STD_STRING);
			rany->setData(destName);

			returnValues.push_back(rany);
		}
	}
}