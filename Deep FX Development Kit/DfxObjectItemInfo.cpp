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

#include "DfxObjectItemInfo.h"


DfxObjectItemInfo::DfxObjectItemInfo(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
	setLogicalName("Object Info");

	using namespace DfxEntityPropertyTypes;

	*(float*)getProperty(outVertexCount) = 0.f;
	*(float*)getProperty(outFaceCount)   = 0.f;

	((Float3*)getProperty(outPosition))->x = .0f;
	((Float3*)getProperty(outPosition))->y = .0f;
	((Float3*)getProperty(outPosition))->z = .0f;

	((Float3*)getProperty(outRotation))->x = .0f;
	((Float3*)getProperty(outRotation))->y = .0f;
	((Float3*)getProperty(outRotation))->z = .0f;

	((Float3*)getProperty(outScale))->x = 1.0f;
	((Float3*)getProperty(outScale))->y = 1.0f;
	((Float3*)getProperty(outScale))->z = 1.0f;

	setPropertyState(inObjectID, DfxEntityPropertyState::INPUT_STATE);

	setPropertyState(outFaceCount, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outVertexCount, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outPosition, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outRotation, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outScale, DfxEntityPropertyState::OUTPUT_STATE);
}

DfxObjectItemInfo::~DfxObjectItemInfo() {

}

void DfxObjectItemInfo::compute() {

	using namespace DfxEntityPropertyTypes;

	if (propertyHasBinding(inObjectID)) {
		
		if (isPropertyBindingDirty(inObjectID)) {

			DfxViewItem *vitem = DfxScene::getInstance()->getItemByID(*(size_t*)getProperty(inObjectID));

			if ( vitem != nullptr &&
				 vitem->getItemType() == DFX_OBJECT_ITEM ) {

				DfxObjectItem *obj_item = (DfxObjectItem*)vitem;

				int vcount = *(float*)getProperty(outVertexCount);

				int fcount  = *(float*)getProperty(outFaceCount);

				*(float*)getProperty(outFaceCount) = 0.f;

				Float3 *_pos = (Float3*)getProperty(outPosition);
				glm::vec3 pos(_pos->x, _pos->y, _pos->z);

				Float3 *_rot = (Float3*)getProperty(outRotation);
				glm::vec3 rot(_rot->x, _rot->y, _rot->z);

				Float3 *_scl = (Float3*)getProperty(outScale);
				glm::vec3 scl(_scl->x, _scl->y, _scl->z);

				glm::vec3 opos = obj_item->getPosition();
				glm::vec3 orot = obj_item->getRotation();
				glm::vec3 oscl = obj_item->getScale();

				if (obj_item->getMeshData()->getVertexCount() != vcount) {
					*(float*)getProperty(outVertexCount) = obj_item->getMeshData()->getVertexCount();
					makePropertyBindingDirty(outVertexCount);
				}

				if ( (obj_item->getMeshData()->getIndexCount()/3) != fcount ) {
					*(float*)getProperty(outFaceCount) = (obj_item->getMeshData()->getIndexCount() / 3);
					makePropertyBindingDirty(outFaceCount);
				}

				if (opos != pos) {
					_pos->x = opos.x; _pos->y = opos.y; _pos->z = opos.z;
					makePropertyBindingDirty(outPosition);
				}

				if (orot != rot) {
					_rot->x = orot.x; _rot->y = orot.y; _rot->z = orot.z;
					makePropertyBindingDirty(outRotation);
				}

				if (oscl != scl) {
					_scl->x = oscl.x; _scl->y = oscl.y; _scl->z = oscl.z;
					makePropertyBindingDirty(outScale);
				}

				makeChildrenDirty();
			}
		}
	}

	getDataInstance()->makeClean();	
}

void DfxObjectItemInfo::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxObjectItemInfo::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxObjectItemInfo::exportData() {
	return mapEntityProperties(this);
}

bool DfxObjectItemInfo::isExportable() {
	return true;
}

void *DfxObjectItemInfo::creator(DfxGlobalEntityName uniquename) {
	return new DfxObjectItemInfo(uniquename);
}

void DfxObjectItemInfo::initialize() {

	DfxEntityDB::createProperty(inObjectID, "object_id", new DfxEntityPropertyTypes::Size_TPropertyType);
	DfxEntityDB::createProperty(outVertexCount, "vertex_count", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(outFaceCount, "face_count", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(outPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(outRotation, "rotation", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(outScale, "scale", new DfxEntityPropertyTypes::Float3PropertyType());
}