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

#include "DfxVoronoiFracture.h"

namespace dfxcore {

	DfxVoronoiFracture::DfxVoronoiFracture(DfxGlobalEntityName uniquename):DfxEntity(uniquename) {
		setLogicalName("Voronoi Fracture");

		DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
		uid01->setRootID(getInstanceID());

		setPropertyState(proxies, DfxEntityPropertyState::DATA_STORE_STATE);

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSeed, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inCellCount, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSamples, DfxEntityPropertyState::INPUT_STATE);

		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
		//setPropertyState(proxies, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inSeed) = 22232.f;
		*(float*)getProperty(inCellCount) = 10.f;

		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.f;
		((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.f;
	}

	DfxVoronoiFracture::~DfxVoronoiFracture() {
			
	}

	void DfxVoronoiFracture::compute() {
		using namespace DfxEntityPropertyTypes;

		ProxyObjectArray *parray = (ProxyObjectArray*)getProperty(proxies);
		parray->deleteArray();

		DfxGlobal::info::message("Voronoi Fracturing Mesh...");

		if (propertyHasBinding(inRef)) {

			dfxutil::traitUIDLinkRelated(this, outRef);
			
			UID *uid = (UID*)getProperty(inRef);

			int seed = (int)*(float*)getProperty(inSeed);
			int cellCount = (int)*(float*)getProperty(inCellCount);
			Float3Array *samples_arr = (Float3Array*)getProperty(inSamples);

			std::vector<glm::vec3> samples;

			if (!samples_arr->isNull()) {
				for (size_t i = 0; i < samples_arr->size(); i++) {

					auto sm = glm::vec3(samples_arr->get(i).x,
						samples_arr->get(i).y,
						samples_arr->get(i).z);

					samples.push_back(sm);
				}
			}

			Dfx3dMeshData *in_mesh = dfxutil::findMeshData(uid);
			ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);

			if (in_mesh != nullptr || pxs != nullptr) {

				std::vector<Dfx3dMeshData *> fragments;
				
				bool result;

				if (in_mesh != nullptr) {
					fragments = fructureMesh(in_mesh, getAbsoluteTransform(), cellCount, seed, samples, result);
				}
				else {
					if (!pxs->isNull()) {
						for (int pinx = 0; pinx < pxs->size(); pinx++) {

							ProxyObject *proxPtr = &(*pxs)[pinx];;

							if (proxPtr != nullptr) {

								auto trans = getAbsoluteTransform(proxPtr);
								std::vector<Dfx3dMeshData *> output = fructureMesh(&proxPtr->meshData, trans, cellCount, seed, samples, result);

								if (output.size() > 0) {

									std::vector<Dfx3dMeshData *> newVec;
									newVec.reserve(output.size() + fragments.size()); 

									newVec.insert(newVec.end(), fragments.begin(), fragments.end());
									newVec.insert(newVec.end(), output.begin(), output.end());

									fragments = newVec;
								}
							}
						}
					}
				}

				if (fragments.size() > 0) {
					parray->resize(fragments.size());

					for (size_t i = 0; i < fragments.size(); i++) {
						(*parray)[i].meshData.copy(*fragments[i]);
						(*parray)[i].meshData.calculateVertexNormals();

						glm::vec3 pos = dfxutil::geo::moveMeshToOrigin((*parray)[i].meshData);

						(*parray)[i].position.x = pos.x;
						(*parray)[i].position.y = pos.y;
						(*parray)[i].position.z = pos.z;

						(*parray)[i].scale.x = 1.0f;
						(*parray)[i].scale.y = 1.0f;
						(*parray)[i].scale.z = 1.0f;

						fragments[i]->clearIndexBuffer();
						fragments[i]->clearVertexBuffer();

						delete fragments[i];
					}

				}

			}
		}

		DfxGlobal::info::message("");

		cleanAllProperties();
		makeChildrenDirty();
		getDataInstance()->makeClean();
	}

	void DfxVoronoiFracture::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxVoronoiFracture::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxVoronoiFracture::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxVoronoiFracture::isExportable() {
		return true;
	}

	void *DfxVoronoiFracture::creator(DfxGlobalEntityName uniquename) {
		return new DfxVoronoiFracture(uniquename);
	}

	void DfxVoronoiFracture::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(proxies, "@proxy_mesh_data", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inSeed, "in_seed", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inCellCount, "in_cells", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPosition,"in_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inSamples, "in_samples", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	glm::mat4 DfxVoronoiFracture::getAbsoluteTransform() {

		DfxEntityPropertyTypes::Float3 position = *(DfxEntityPropertyTypes::Float3*)getProperty(inPosition);
		DfxEntityPropertyTypes::Float3 rotation = *(DfxEntityPropertyTypes::Float3*)getProperty(inRotation);
		DfxEntityPropertyTypes::Float3 scale = *(DfxEntityPropertyTypes::Float3*)getProperty(inScale);

		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		trans *= glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));

		return trans;
	}

	glm::mat4 DfxVoronoiFracture::getAbsoluteTransform(DfxEntityPropertyTypes::ProxyObject *prox) {

		DfxEntityPropertyTypes::Float3 position = prox->position;
		DfxEntityPropertyTypes::Float3 rotation = prox->rotation;
		DfxEntityPropertyTypes::Float3 scale	= prox->scale;

		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		trans *= glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));

		return trans;
	}

	std::vector<Dfx3dMeshData*> DfxVoronoiFracture::fructureMesh( Dfx3dMeshData *mesh, glm::mat4 trans, int32_t cellCount, int32_t seed, std::vector<glm::vec3> samples, bool &success ) {

		std::vector<Dfx3dMeshData *> fragments;
		
		if (mesh != nullptr) {
			if (mesh->getVertexCount() >= 3 && mesh->getIndexCount() >= 3) {

				dfxblast::VoronoiOptions opts;
				opts.cell_count = cellCount;
				opts.rand_seed = seed;
	
				try {
					fragments = dfxblast::DfxNvBlastGlobal::getInstance()->voronoiFracture(mesh, opts, trans, samples);
					success = true;
				}
				catch (DfxNvBlastException const &e) {
					DfxGlobal::console::println(e.what());
					success = false;
				}
			}
		}

		return fragments;
	}
}