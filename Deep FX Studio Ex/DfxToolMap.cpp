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

#include "DfxToolMap.h"


DfxToolMap *DfxToolMap::_instance = nullptr;

DfxToolMap::DfxToolMap() {
	root = DfxToolMapNode::Root();
}

DfxToolMap *DfxToolMap::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxToolMap;
	}

	return _instance;
}

std::vector<DfxToolMapNode*> DfxToolMap::domains() {
	return root->getChildren();
}

DfxToolMapNode *DfxToolMap::addDomain(std::string name, std::string resource, bool active) {
	return root->addDomain(name, resource, active);
}