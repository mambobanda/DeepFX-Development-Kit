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

#include "DfxInstanceManager.h"

DfxInstanceManager *DfxInstanceManager::_instance = nullptr;

DfxInstanceManager::DfxInstanceManager() {

}

DfxInstanceManager::~DfxInstanceManager() {

}

DfxInstanceManager *DfxInstanceManager::getInstance() {
	
	if (_instance == nullptr) {
		_instance = new DfxInstanceManager;
	}

	return _instance;
}

void DfxInstanceManager::addInstancer(DfxInstancerBase *inst) {
	_intancers[inst->id()] = inst;
}

void DfxInstanceManager::removeInstancer(DfxInstancerBase *inst) {
	auto iter = _intancers.find(inst->id());

	if (iter != _intancers.end()) _intancers.erase(iter);

}

std::vector<DfxInstancerBase*> DfxInstanceManager::intancers() {
	
	std::vector<DfxInstancerBase*> output;

	for (auto p: _intancers) {
		output.push_back(p.second);
	}

	return output;
}