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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <DfxGlobal.h>
#include <FlSpace.h>
#include <FlInterpreterScope.h>
#include <FlModel.h>
#include <Octree.hpp>
#include <set>
#include "DfxFluentSet.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"

namespace compute_node {

	static const std::string proximity_model_id = "/COMPUTE/PROXIMITY_NODE";

	class ProximityNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void proximityMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0] [1], expected string";

					return;
				}

				if (args[2]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2], expected float";

					return;
				}

				if (args[3]->type() != fl::type::BOOL || 
					args[4]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [3] [4], expected bool";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[3]->isArray() || args[4]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto set_a = args[0]->data<std::string>();
				auto set_b = args[1]->data<std::string>();
				auto radius   = args[2]->data<float>();
				auto nearest  = args[3]->data<bool>();
				auto furthest = args[4]->data<bool>();

				
				auto softResultset = DfxFluentSetManager::getInstance()->createSoftSet(DfxFluentSet::EMPTY);
				std::string output = softResultset.first;

				if (!set_a.empty() && !set_b.empty()) {

					auto sset_a = DfxFluentSetManager::getInstance()->getSet(set_a);
					auto sset_b = DfxFluentSetManager::getInstance()->getSet(set_b);

					if (sset_a != nullptr && sset_b != nullptr) {

						glm::vec3 *positions_a = nullptr;
						glm::mat4 *xform_a = nullptr;
						glm::vec3 *positions_b = nullptr;
						glm::mat4 *xform_b = nullptr;

						if (sset_a->type() == DfxFluentSet::SIMPLEX) {
							positions_a = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
							xform_a = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform");
						}
						else if (sset_a->type() == DfxFluentSet::STREAM) {
							positions_a = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
						}

						if (sset_b->type() == DfxFluentSet::SIMPLEX) {
							positions_b = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
							xform_b = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform");
						}
						else if (sset_b->type() == DfxFluentSet::STREAM) {
							positions_b = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
						}

						if (radius > 0.f) {
							std::vector<glm::vec3> point_seta;
							std::vector<glm::vec3> point_setb;

							unibn::Octree<glm::vec3> octree;
							unibn::OctreeParams params;

							for (size_t i = 0; i < sset_a->indices().size(); i++) {
								glm::vec3 pos(0.f);
								if (xform_a == nullptr) {
									pos = positions_a[sset_a->indices()[i]];
								}
								else {
									pos = positions_a[sset_a->indices()[i]];
									pos = glm::vec3(xform_a[sset_a->indices()[i]] * glm::vec4(pos, 1.f));
								}

								point_seta.push_back(pos);
							}

							for (size_t i = 0; i < sset_b->indices().size(); i++) {
								glm::vec3 pos(0.f);
								if (xform_b == nullptr) {
									pos = positions_b[sset_b->indices()[i]];
								}
								else {
									pos = positions_b[sset_b->indices()[i]];
									pos = glm::vec3(xform_b[sset_b->indices()[i]] * glm::vec4(pos, 1.f));
								}

								point_setb.push_back(pos);
							}

							octree.initialize(point_setb);

							std::set<unsigned int> result_set;
							for (unsigned int i = 0; i < point_seta.size(); ++i) {
								std::vector<uint32_t> results;
								octree.radiusNeighbors<unibn::L2Distance<glm::vec3>>(point_seta[i], radius, results);

								for (unsigned int j = 0; j < results.size(); ++j) 
									result_set.insert(sset_b->indices()[results[j]]);
							}

							softResultset.second->setType(sset_b->type());
							for (auto ri : result_set)softResultset.second->indices().push_back(ri);

						}
						else {
							error->hasError = true;
							error->errorMsg = "Radius must be bigger than 0.";

							return;
						}
					}
					else {
						error->hasError = true;
						error->errorMsg = "Set A or Set B Doesn't Exist.";

						return;
					}
				}

				auto rany = new fl::Any();
				rany->newData<std::string>(fl::type::STD_STRING);
				rany->setData(output);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *proximityNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("proximity")
				.setLabel("Proximity")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(proximity_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "nei", "Name", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set_a", "Set A", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set_b", "Set B", std::string())
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "radius", "Radius", 0.5f)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "closest", "Closest", false)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "furthest", "Furtherest", false)

				.addStaticFunction(fl::DataObject::STATIC_MAIN, proximityMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return proximityNode(space);
		}
	};

	inline void registerProximityNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = proximity_model_id;
		desc->name = "Proximity";
		desc->description = "Find closest points in set b.";
		desc->category = "Sets";
		desc->placement = "Sets/Proximity";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ProximityNodeHandler::proximityNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}