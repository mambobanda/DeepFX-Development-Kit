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

#include "DfxMaterialLinker.h"

DfxMaterialLinker *DfxMaterialLinker::_instance = nullptr;


DfxMaterialLinker::DfxMaterialLinker() {

}

DfxMaterialLinker::~DfxMaterialLinker() {

}

DfxMaterialLinker *DfxMaterialLinker::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxMaterialLinker;
	}

	return _instance;
}

void DfxMaterialLinker::updateObjectLink(std::string obj_path, unsigned int id, std::string mat_path) {
	auto itr = _map.find(obj_path);

	DfxMaterialIDMap *lmap = nullptr;
	if (itr != _map.end()) {
		lmap = itr->second;
	}
	else {
		lmap = new DfxMaterialIDMap;
		_map[obj_path] = lmap;
	}

	lmap->insert(id, mat_path);
}

void DfxMaterialLinker::removeObjectLink(std::string obj_path, unsigned int id) {
	auto itr = _map.find(obj_path);

	if (itr != _map.end()) {
		itr->second->remove(id);

		if (itr->second->size() == 0) {
			delete itr->second;
			_map.erase(obj_path);
		}
	}
}

void DfxMaterialLinker::removeAllLinks(std::string obj_path) {
	auto itr = _map.find(obj_path);

	if (itr != _map.end()) {
		_map.erase(itr);
	}
}

void DfxMaterialLinker::removeAllMaterialLinks(std::string mat_path) {
	std::vector<std::pair<std::string, unsigned int>> links;

	for (auto p00 : _map) {
		for (auto p01 : p00.second->all()) {
			if (p01.second == mat_path) {
				links.push_back(std::pair<std::string, unsigned int>(p00.first, p01.first));
			}
		}
	}

	for (auto link: links) {
		removeObjectLink(link.first, link.second);
	}
}

std::unordered_map<std::string, DfxMaterialIDMap*> &DfxMaterialLinker::materialLinkMap() {
	return _map;
}

std::vector<std::pair<unsigned int, std::string>> DfxMaterialLinker::allObjectLinks(std::string obj_path) {
	std::vector<std::pair<unsigned int, std::string>> output;

	auto itr = _map.find(obj_path);

	if (itr != _map.end()) {
		return itr->second->all();
	}

	return output;
}

bool DfxMaterialLinker::hasObjectLink(std::string obj_path, unsigned int id) {
	auto itr = _map.find(obj_path);

	if (itr != _map.end()) {
		return itr->second->hasID(id);
	}

	return false;
}

std::string DfxMaterialLinker::objectLink(std::string obj_path, unsigned int id) {
	auto itr = _map.find(obj_path);

	if (itr != _map.end()) {
		return itr->second->path(id);
	}

	return "";
}

void DfxMaterialLinker::clear() {

	for (auto &p: _map) {
		delete p.second;
	}

	_map.clear();
}

void DfxMaterialLinker::clearCallback() {
	getInstance()->clear();
}