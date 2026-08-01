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

#include "DfxObjectItemCopy.h"


DfxObjectItemCopy::DfxObjectItemCopy(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
	setLogicalName("Copy Mesh");

	DfxEntityPropertyTypes::UID *uid01 = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
	uid01->setRootID(getInstanceID());

	setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inRotations, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inScales, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(outProxies, DfxEntityPropertyState::DATA_STORE_STATE);
	
}

DfxObjectItemCopy::~DfxObjectItemCopy() {

}

void DfxObjectItemCopy::compute() {
	using namespace DfxEntityPropertyTypes;

	if (propertyHasBinding(inPoints) && propertyHasBinding(inRef)) {

		dfxutil::traitUIDLinkRelated(this, outRef);

		Float3Array *points = (Float3Array*)getProperty(inPoints);
		Float3Array *rotations = (Float3Array*)getProperty(inRotations);
		Float3Array *scales = (Float3Array*)getProperty(inScales);
		UID *uid = (UID*)getProperty(inRef);
		ProxyObjectArray *pxs = (ProxyObjectArray*)getProperty(outProxies);

		pxs->deleteArray();

		Dfx3dMeshData* imsh = dfxutil::findMeshData(uid);;

		if (!points->isNull() && imsh != nullptr) {

			if (points->size() > 0) {
				pxs->resize(points->size());

				for (int inx = 0; inx < points->size(); inx++) {
					(*pxs)[inx].meshData.copy(*imsh);
					(*pxs)[inx].position = (*points)[inx];

					// 
					if (propertyHasBinding(inRotations) && !rotations->isNull() && rotations->size() > 0 &&
						inx < rotations->size()) {
						(*pxs)[inx].rotation = (*rotations)[inx];
					}
					else {
						(*pxs)[inx].rotation.x = 0.f;
						(*pxs)[inx].rotation.y = 0.f;
						(*pxs)[inx].rotation.z = 0.f;
					}

					// 
					if (propertyHasBinding(inScales) && !scales->isNull() && scales->size() > 0 &&
						inx < scales->size()) {
						(*pxs)[inx].scale = (*scales)[inx];
					}
					else {
						(*pxs)[inx].scale.x = 1.f;
						(*pxs)[inx].scale.y = 1.f;
						(*pxs)[inx].scale.z = 1.f;
					}
					
				}

				makePropertyBindingDirty(outProxies);
				makeChildrenDirty();				
			}
		}
	}
	else{
		
		ProxyObjectArray *pxs = (ProxyObjectArray*)getProperty(outProxies);
		pxs->deleteArray();
	}
	
	getDataInstance()->makeClean();
}

void DfxObjectItemCopy::onDataChanged(DfxEntityDataInstance *instance) {

}

Dfx3dMeshData *DfxObjectItemCopy::getMeshData(size_t inx) {
	using namespace DfxEntityPropertyTypes;

	ProxyObjectArray *pxs = (ProxyObjectArray*)getProperty(outProxies);

	if (!pxs->isNull()) {
		
		if (inx >= 0 && inx < pxs->size()) {
			return &(*pxs)[inx].meshData;
		}
	}

	return nullptr;
}

void DfxObjectItemCopy::importData(std::vector<std::pair<std::string, std::string>> data) {

}


std::vector<std::pair<std::string, std::string>> DfxObjectItemCopy::exportData() {
	return mapEntityProperties(this);
}

bool DfxObjectItemCopy::isExportable() {
	return true;
}

void *DfxObjectItemCopy::creator(DfxGlobalEntityName uniquename) {
	return new DfxObjectItemCopy(uniquename);
}

void DfxObjectItemCopy::initialize() {
	DfxViewItem::initialize();

	DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inPoints,"positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	DfxEntityDB::createProperty(inRotations, "rotations", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	DfxEntityDB::createProperty(inScales, "scales", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	DfxEntityDB::createProperty(outProxies, "@proxy_mesh_data", new DfxEntityPropertyTypes::ProxyObjectArrayPropertyType());
	
}