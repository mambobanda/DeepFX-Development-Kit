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

#include "DfxRenderManager.h"


DfxRenderManager *DfxRenderManager::_instance = nullptr;


DfxRenderManager::DfxRenderManager() {

}

DfxRenderManager *DfxRenderManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxRenderManager;
	}

	return _instance;
}

void DfxRenderManager::addRenderSessionCallback(unsigned int id, DfxRenderSessionInstance cb) {
	_instance_map[id] = cb;
}

void DfxRenderManager::removeRenderSessionCallback(unsigned int id) {
	auto it = _instance_map.find(id);

	if (it != _instance_map.end()) _instance_map.erase(it);
}

DfxRenderSessionInterface *DfxRenderManager::newRenderSession(unsigned int id) {
	auto it = _instance_map.find(id);

	if (it != _instance_map.end()) {
		return it->second();
	}
	else {
		return nullptr;
	}

}
