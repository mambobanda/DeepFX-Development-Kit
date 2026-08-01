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

#include "DfxSliceFracture.h"

namespace dfxcore {

	DfxSliceFracture::DfxSliceFracture(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
		setLogicalName("Slice Fracture");

		DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
		uid01->setRootID(getInstanceID());

		setPropertyState(proxies, DfxEntityPropertyState::DATA_STORE_STATE);

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inXSlices, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inYSlices, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inZSlices, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inAngleVariation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inOffsetVariation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);

		setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inXSlices) = 5.f;
		*(float*)getProperty(inYSlices) = 5.f;
		*(float*)getProperty(inZSlices) = 5.f;
	}

	DfxSliceFracture::~DfxSliceFracture() {

	}

	void DfxSliceFracture::compute() {
		using namespace DfxEntityPropertyTypes;

		ProxyObjectArray *parray = (ProxyObjectArray*)getProperty(proxies);
		parray->deleteArray();

		DfxGlobal::info::message("Slice Fracturing Mesh...");

		if (propertyHasBinding(inRef)) {

			dfxutil::traitUIDLinkRelated(this, outRef);

			UID *uid = (UID*)getProperty(inRef);

			int xs = (int)*(float*)getProperty(inXSlices);
			int ys = (int)*(float*)getProperty(inYSlices);
			int zs = (int)*(float*)getProperty(inZSlices);
			float av = *(float*)getProperty(inAngleVariation);
			float ov = *(float*)getProperty(inOffsetVariation);

			Dfx3dMeshData *in_mesh = dfxutil::findMeshData(uid);
			ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);

			if (in_mesh != nullptr || pxs != nullptr) {

				std::vector<Dfx3dMeshData *> fragments;

				bool result;

				if (in_mesh != nullptr) {
					fragments = fructureMesh(in_mesh, getAbsoluteTransform(), xs, ys, zs, av, ov, result);
				}
				else {
					if (!pxs->isNull()) {
						for (int pinx = 0; pinx < pxs->size(); pinx++) {

							ProxyObject *proxPtr = &(*pxs)[pinx];;

							if (proxPtr != nullptr) {

								auto trans = getAbsoluteTransform(proxPtr);
								std::vector<Dfx3dMeshData *> output = fructureMesh(in_mesh, getAbsoluteTransform(), xs, ys, zs, av, ov, result);

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

	void DfxSliceFracture::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxSliceFracture::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxSliceFracture::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxSliceFracture::isExportable() {
		return true;
	}

	void *DfxSliceFracture::creator(DfxGlobalEntityName uniquename) {
		return new DfxSliceFracture(uniquename);
	}

	void DfxSliceFracture::initialize() {

		DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(proxies, "@proxy_mesh_data", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inXSlices, "x_slices", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inYSlices, "y_slices", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inZSlices, "z_slices", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inAngleVariation, "angle_variation", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inOffsetVariation, "offset_variation", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPosition, "in_position", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
	}

	glm::mat4 DfxSliceFracture::getAbsoluteTransform() {

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

	glm::mat4 DfxSliceFracture::getAbsoluteTransform(DfxEntityPropertyTypes::ProxyObject *prox) {

		DfxEntityPropertyTypes::Float3 position = prox->position;
		DfxEntityPropertyTypes::Float3 rotation = prox->rotation;
		DfxEntityPropertyTypes::Float3 scale = prox->scale;

		glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));

		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		trans *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		trans *= glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));

		return trans;
	}

	std::vector<Dfx3dMeshData*> DfxSliceFracture::fructureMesh( Dfx3dMeshData *mesh, glm::mat4 trans, float xslice, float yslice, float zslice, float aVariation, float oVariation, bool &success ) {

		std::vector<Dfx3dMeshData *> fragments;

		if (mesh != nullptr) {
			if (mesh->getVertexCount() >= 3 && mesh->getIndexCount() >= 3) {

				dfxblast::SliceOptions opts;
				opts.x_slices = xslice;
				opts.y_slices = yslice;
				opts.z_slices = zslice;
				opts.angleVariation = aVariation;
				opts.offsetVariation = oVariation;

				try {
					fragments = dfxblast::DfxNvBlastGlobal::getInstance()->sliceFracture(mesh, opts, trans);
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