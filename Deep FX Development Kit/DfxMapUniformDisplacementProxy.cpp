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

#include "DfxMapUniformDisplacementProxy.h"


DfxMapUniformDisplacementProxy::DfxMapUniformDisplacementProxy(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
	setLogicalName("Map Uniform Displacement");

	setPropertyState(inProxies, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inTrigger, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inFrame, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inTransforms, DfxEntityPropertyState::INPUT_STATE);
}

DfxMapUniformDisplacementProxy::~DfxMapUniformDisplacementProxy() {

}

void DfxMapUniformDisplacementProxy::compute() {
	using namespace DfxEntityPropertyTypes;

	if (propertyHasBinding(inProxies) && propertyHasBinding(inTransforms)) {

		ProxyObjectArray *proxies  = (ProxyObjectArray*)getProperty(inProxies);
		Matrix4x4Array *transforms = (Matrix4x4Array*)getProperty(inTransforms);

		if (!proxies->isNull() && proxies->size() > 0) {

			for (size_t pinx = 0; pinx < proxies->size(); pinx++) {

				ProxyObject *obj = &(*proxies)[pinx];
				Matrix4x4 temp_mat;

				if (!transforms->isNull() && pinx < transforms->size()) {
					temp_mat.copy((*transforms)[pinx]);
				}

				if (obj->meshData.getVertexCount() > 0) {

					int frame = *(float*)getProperty(inFrame);
					DfxEntityPropertyTypes::Matrix4x4 tempTrans = temp_mat;
					glm::mat4 trans = glm::make_mat4(tempTrans.m);

					DfxObjectDisplacement *disp = new DfxObjectDisplacement;
					disp->init(obj->meshData.getVertexCount());

					for (size_t inx = 0; inx < obj->meshData.getVertexCount(); inx++) {

						glm::vec3 vert = obj->meshData.getVertex(inx);

						vert = glm::vec3(trans * glm::vec4(vert * float3ToVec3(obj->scale), 1.f));

						disp->getPositions()[inx] = vert;

						/*if (inx == 0) {
						std::stringstream ss;
						ss << vert.x << ", " << vert.y << ", " << vert.z;
						DfxGlobal::console::println(ss.str());
						}*/
					}

					obj->displacementMap->insertDisplacement(frame, disp);
				}		
			}
		}
	}

	getDataInstance()->makeClean();
}

void DfxMapUniformDisplacementProxy::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxMapUniformDisplacementProxy::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxMapUniformDisplacementProxy::exportData() {
	return mapEntityProperties(this);
}

bool DfxMapUniformDisplacementProxy::isExportable() {
	return true;
}

void *DfxMapUniformDisplacementProxy::creator(DfxGlobalEntityName uniquename) {
	return new DfxMapUniformDisplacementProxy(uniquename);
}

void DfxMapUniformDisplacementProxy::initialize() {

	DfxEntityDB::createProperty(inProxies, "proxies", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType());
	DfxEntityDB::createProperty(inTrigger, "trigger", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inFrame, "frame", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inTransforms, "transforms4x4", new DfxEntityPropertyTypes::DfxMatrix4x4ArrayPropertyType());
}