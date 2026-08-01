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

#include "DfxSurfaceDictionary.h"


DfxSurfaceDictionary *DfxSurfaceDictionary::_instance = nullptr;

DfxSurfaceDictionary::DfxSurfaceDictionary() {

}

DfxSurfaceDictionary::~DfxSurfaceDictionary() {

}

DfxSurfaceDictionary *DfxSurfaceDictionary::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxSurfaceDictionary;
	}

	return _instance;
}

unsigned int DfxSurfaceDictionary::maxID(std::string path) {
	unsigned max = 0;

	auto itr = _map.find(path);

	if (itr != _map.end()) {
		for (auto &p : itr->second) {
			if (p.first < ID_LIMIT) {
				max = std::max(max, p.first);
			}
		}
	}

	return max;
}

std::string DfxSurfaceDictionary::string(std::string path, unsigned int id) {

	auto itr = _map.find(path);

	if (itr != _map.end()) {
		auto itr_name = itr->second.find(id);
		if (itr_name != itr->second.end()) return itr_name->second;
	}

	return "";
}

std::vector<unsigned int> DfxSurfaceDictionary::find(std::string path, std::string str) {
	std::vector<unsigned int> output;

	/*for (auto p : _map) {

	}*/

	return output;
}

void DfxSurfaceDictionary::create(std::string path) {

	auto itr = _map.find(path);

	if (itr != _map.end()) {
		_map[path] = std::unordered_map<unsigned int, std::string>();
	}

}

void DfxSurfaceDictionary::insert(std::string path, unsigned int id, std::string name) {

	auto itr = _map.find(path);

	if (itr != _map.end()) {
		itr->second[id] = name;
	}
	else {
		_map[path] = std::unordered_map<unsigned int, std::string>();
		_map[path][id] = name;
	}
}

void DfxSurfaceDictionary::erase(std::string path, unsigned int id) {
	auto itr = _map.find(path);

	if (itr != _map.end()) {
		auto itr_0 = itr->second.find(id);
		if (itr_0 != itr->second.end()) itr->second.erase(id);

		if (itr->second.empty()) _map.erase(itr);
	}
	
}

void DfxSurfaceDictionary::eraseAll(std::string path) {
	auto itr = _map.find(path);

	if (itr != _map.end()) {
		_map.erase(itr);
	}
}

std::vector<std::pair<std::string, std::unordered_map<unsigned int, std::string>>> DfxSurfaceDictionary::all() {
	std::vector<std::pair<std::string, std::unordered_map<unsigned int, std::string>>> output;

	for (auto p : _map) output.push_back(p);

	return output;
}

void DfxSurfaceDictionary::clear() {
	_map.clear();
}

void DfxSurfaceDictionary::clearCallback() {
	getInstance()->clear();
}