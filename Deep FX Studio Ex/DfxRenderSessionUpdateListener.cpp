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

#include "DfxRenderSessionUpdateListener.h"


DfxRenderSessionUpdateListener::DfxRenderSessionUpdateListener(DfxRenderSessionInterface *inter, QObject *parent): QObject(parent) {
	_sessionInterface = inter;

	QObject::connect(DfxUpdater::getInstance(), &DfxUpdater::_nodeTransformUpdated, this, &DfxRenderSessionUpdateListener::transformUpdated);
	QObject::connect(DfxUpdater::getInstance(), &DfxUpdater::_materialUpdated, this, &DfxRenderSessionUpdateListener::materialUpdated);
}

DfxRenderSessionUpdateListener::~DfxRenderSessionUpdateListener() {

}

void DfxRenderSessionUpdateListener::transformUpdated(std::vector<deepsg::SgNode*> nodes) {
	bool updateGeo = false;
	bool updateLight = false;
	bool updateCamera = false;

	for (auto node: nodes) {
		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
			updateGeo = true;
		}
		else if (node->getType() == deepsg::SgNodeType::SHAPE) {
			if (node->uniqueName() == "UID_ENVIROMENT_LIGHT_WIDGET"   || 
				node->uniqueName() == "UID_LUX_INFINITE_LIGHT_WIDGET" || 
				node->uniqueName() == "UID_LUX_POINT_LIGHT_WIDGET"    || 
				node->uniqueName() == "UID_SUN_SKY_LIGHT_WIDGET") {

				updateLight = true;
			}
			else if (node->uniqueName() == "UID_CAMERA_WIDGET") {

				updateCamera = true;
			}
		}
	}

	if (updateLight) {
		_sessionInterface->updateLights();
	}

	if (updateCamera) {
		_sessionInterface->updateCamera();
	}

	if (updateGeo) {
		_sessionInterface->updateGeometryTransform();
	}

}

void DfxRenderSessionUpdateListener::materialUpdated() {
	_sessionInterface->updateMaterials();
}
