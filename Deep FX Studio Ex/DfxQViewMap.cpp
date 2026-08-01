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

#include "DfxQViewMap.h"


DfxQViewMap *DfxQViewMap::_instance = nullptr;


DfxQViewMap::DfxQViewMap() {

}

DfxQViewMap::~DfxQViewMap() {

}

DfxQViewMap *DfxQViewMap::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxQViewMap;
	}

	return _instance;
}

void DfxQViewMap::addEntry(std::string name, DfxQViewID id, std::function<QWidget*()> func) {
	_map.push_back(std::pair<std::string, std::pair<DfxQViewID, std::function<QWidget*()>>>(name, std::pair<DfxQViewID, std::function<QWidget*()>>(id, func)));
}
	
std::function<QWidget*()> DfxQViewMap::findEntry(DfxQViewID id, bool &found) {
	found = false;

	for (auto &e : _map) {

		if (id == e.second.first) {
			found = true;
			return e.second.second;
		}
	}

	return []() { return nullptr; };
}

std::string DfxQViewMap::findEntryName(DfxQViewID id) {

	for (auto &e : _map) {

		if (id == e.second.first) {
			return e.first;
		}
	}

	return "N/A";
}


std::vector<std::pair<std::string, std::pair<DfxQViewID, std::function<QWidget*()>>>> &DfxQViewMap::entries() {
	return _map;
}