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

#include "DfxPythonObjectInstanceMap.h"
#include "DfxPythonFunctions.h"
#include "DfxLayerManager.h"

void buildPythonNode(DfxGenericPythonEntity &ent) {

	ent.buildInstance();

	py::object *obj = new py::object;
	*obj = py::cast(ent);
	DfxPythonObjectInstanceMap::add(ent.getInstanceID(), obj);

	DfxLayerManager::getInstance()->addNode(ent.castToBase());
	
}

void destroyPythonNode(size_t instanceID) {
	auto layer = DfxLayerManager::getInstance()->findNodeByID(instanceID);

	if (layer != nullptr) {
		DfxLayerManager::getInstance()->removeNode(layer->getNode(), false);
	}
	else {
		std::cout << "Failed to destroy python node.\n";
	}
}