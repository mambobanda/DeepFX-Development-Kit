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

#include "DfxFlowDB.h"

size_t DfxFlowDB::ID_GEN = 0x3e8;
DfxFlowDB *DfxFlowDB::_instance = nullptr;

DfxFlowDB::DfxFlowDB() {

}

DfxFlowDB::~DfxFlowDB() {

}

DfxFlowDB *DfxFlowDB::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxFlowDB;
	}

	return _instance;
}

void DfxFlowDB::addFlow(size_t id, fl::Flow *flow) {
	_map[id] = flow;
}

void DfxFlowDB::removeFlow(size_t id) {
	_map.erase(id);
}

fl::Flow *DfxFlowDB::findFlow(size_t id) {
	auto itr = _map.find(id);

	if (itr != _map.end()) {
		return itr->second;
	}

	return nullptr;
}

void DfxFlowDB::resetMaxID() {

	ID_GEN = 0x3e8;
	for (auto p : _map) {
		ID_GEN = std::max(ID_GEN, p.first);
	}
	
}

size_t DfxFlowDB::newID() {
	return ++ID_GEN;
}

void DfxFlowDB::setID(size_t new_id) {
	ID_GEN = new_id;
}

std::vector<std::pair<size_t, fl::Flow*>> DfxFlowDB::flowList() {
	std::vector<std::pair<size_t, fl::Flow*>> output;

	for (auto p: _map) {
		output.push_back(p);
	}

	return output;
}

void DfxFlowDB::reset() {
	for (auto p : _map) {
		delete p.second;
	}

	_map.clear();
	resetMaxID();
}

void DfxFlowDB::resetCallback() {
	DfxFlowDB::getInstance()->reset();
}
