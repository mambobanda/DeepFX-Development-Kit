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

#include "DfxComputeNodeDB.h"

DfxComputeNodeDB *DfxComputeNodeDB::_instance = nullptr;

DfxComputeNodeDB::DfxComputeNodeDB() {

}

DfxComputeNodeDB::~DfxComputeNodeDB() {

}

DfxComputeNodeDB *DfxComputeNodeDB::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxComputeNodeDB;
	}

	return _instance;
}

void DfxComputeNodeDB::registerComputeNode(DfxComputeNodeDescription *desc) {
	_computeNodeMap[desc->identifier] = desc;
}

DfxComputeNodeDescription *DfxComputeNodeDB::findComputeNodeDescription(std::string id) {
	auto itr = _computeNodeMap.find(id);

	if (itr != _computeNodeMap.end()) {
		return itr->second;
	}

	return nullptr;
}

std::vector<DfxComputeNodeDescription*> DfxComputeNodeDB::findAll() {
	std::vector<DfxComputeNodeDescription*> output;

	for (auto p : _computeNodeMap) output.push_back(p.second);

	return output;
}