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

#include "DfxMapUniformDisplacement.h"


DfxMapUniformDisplacement::DfxMapUniformDisplacement(DfxGlobalEntityName uniquename):DfxEntity(uniquename) {
	setLogicalName("Map Uniform Displacement");

	setPropertyState(inObjectID,  DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inTrigger,   DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inFrame,     DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inTransform, DfxEntityPropertyState::INPUT_STATE);
}

DfxMapUniformDisplacement::~DfxMapUniformDisplacement() {

}

void DfxMapUniformDisplacement::compute() {

	if (propertyHasBinding(inObjectID)) {

		size_t objectid = *(size_t*)getProperty(inObjectID);

		DfxViewItem *item = DfxScene::getInstance()->getItemByID(objectid);
		
		if (item != nullptr && item->getGlobalID() == DfxEntity::hash(DfxObjectItem_Identifier)) {

			DfxObjectItem *obj = (DfxObjectItem*)item;

			if (obj->getMeshData()->getVertexCount() > 0) {

				int frame = *(float*)getProperty(inFrame);
				DfxEntityPropertyTypes::Matrix4x4 tempTrans = *(DfxEntityPropertyTypes::Matrix4x4*)getProperty(inTransform);
				glm::mat4 trans = glm::make_mat4(tempTrans.m);

				DfxObjectDisplacement *disp = new DfxObjectDisplacement;
				disp->init(obj->getMeshData()->getVertexCount());

				for (size_t inx = 0; inx < obj->getMeshData()->getVertexCount(); inx++) {

					glm::vec3 vert = obj->getMeshData()->getVertex(inx);

					vert = glm::vec3(trans * glm::vec4(vert * obj->getScale(), 1.f));

					disp->getPositions()[inx] = vert;

					/*if (inx == 0) {
						std::stringstream ss;
						ss << vert.x << ", " << vert.y << ", " << vert.z;
						DfxGlobal::console::println(ss.str());
					}*/
				}

				obj->getDisplacementMap()->insertDisplacement(frame, disp);
			}
		}
	}

	getDataInstance()->makeClean();
}

void DfxMapUniformDisplacement::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxMapUniformDisplacement::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxMapUniformDisplacement::exportData() {
	return mapEntityProperties(this);
}

bool DfxMapUniformDisplacement::isExportable() {
	return true;
}

void *DfxMapUniformDisplacement::creator(DfxGlobalEntityName uniquename) {
	return new DfxMapUniformDisplacement(uniquename);
}

void DfxMapUniformDisplacement::initialize() {

	DfxEntityDB::createProperty(inObjectID, "object_id", new DfxEntityPropertyTypes::Size_TPropertyType());
	DfxEntityDB::createProperty(inTrigger, "trigger", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inFrame, "frame", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inTransform, "transform4x4", new DfxEntityPropertyTypes::DfxMatrix4x4PropertyType());
}