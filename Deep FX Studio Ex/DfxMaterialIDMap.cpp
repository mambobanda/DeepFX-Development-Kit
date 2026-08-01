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

#include "DfxMaterialIDMap.h"

DfxMaterialIDMap::DfxMaterialIDMap() {

}

DfxMaterialIDMap::~DfxMaterialIDMap() {

}

std::vector<std::pair<unsigned int, std::string>> DfxMaterialIDMap::all() {
	std::vector<std::pair<unsigned int, std::string>> output;

	for (auto &p : _map) {
		std::pair<unsigned int, std::string> p(p.first, p.second);
		output.push_back(p);
	}

	return output;
}

std::string DfxMaterialIDMap::path(unsigned int id) {
	auto itr = _map.find(id);

	if (itr != _map.end()) {
		return itr->second;
	}

	return "";
}

bool DfxMaterialIDMap::hasID(unsigned int id) {
	auto itr = _map.find(id);

	if (itr != _map.end()) {
		return true;
	}

	return false;
}

void DfxMaterialIDMap::insert(unsigned int id, std::string path) {
	_map[id] = path;
}

void DfxMaterialIDMap::remove(unsigned int id) {
	auto itr = _map.find(id);

	if (itr != _map.end()) {
		_map.erase(itr);
	}
}

size_t DfxMaterialIDMap::size() {
	return _map.size();
}