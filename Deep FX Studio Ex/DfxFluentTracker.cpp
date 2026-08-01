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

#include "DfxFluentTracker.h"

DfxFluentTracker *DfxFluentTracker::_instance = nullptr;

DfxFluentTracker::DfxFluentTracker() {
}

DfxFluentTracker::~DfxFluentTracker() {

}

DfxFluentTracker *DfxFluentTracker::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxFluentTracker;
	}

	return _instance;
}

std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> &DfxFluentTracker::sceneNodeTracker() {
	return _sceneNodeTracker;
}

std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> &DfxFluentTracker::simplexNodeTracker() {
	return _simplexNodeTracker;
}

void DfxFluentTracker::trackNode(std::string path, std::vector<std::pair<size_t, size_t>> indices) {
	auto itr = _sceneNodeTracker.find(path);

	if (itr != _sceneNodeTracker.end()) {
		for (size_t i = 0; i < indices.size(); i++) {
			itr->second.push_back(indices[i]);
		}
	}
	else {
		_sceneNodeTracker[path] = indices;
	}
}

void DfxFluentTracker::untrackNode(std::string path, std::vector<std::pair<size_t, size_t>> indices) {
	auto itr = _sceneNodeTracker.find(path);

	if (itr != _sceneNodeTracker.begin()) {
		for (size_t i = 0; i < indices.size(); i++) {
			auto sr = itr->second.begin();
			while (sr != itr->second.end()) {
				if ((*sr) == indices[i]) sr = itr->second.erase(sr);
				else sr++;
			}
		}
	}
}

void DfxFluentTracker::trackSimplex(size_t id, std::vector<std::pair<size_t, size_t>> indices) {
	auto itr = _simplexNodeTracker.find(id);

	if (itr != _simplexNodeTracker.begin()) {
		for (size_t i = 0; i < indices.size(); i++) {
			itr->second.push_back(indices[i]);
		}
	}
	else {
		_simplexNodeTracker[id] = indices;
	}
}

void DfxFluentTracker::untrackSimplex(size_t id, std::vector<std::pair<size_t, size_t>> indices) {
	auto itr = _simplexNodeTracker.find(id);

	if (itr != _simplexNodeTracker.begin()) {
		for (size_t i = 0; i < indices.size(); i++) {
			auto sr = itr->second.begin();
			while (sr != itr->second.end()) {
				if ((*sr) == indices[i]) sr = itr->second.erase(sr);
				else sr++;
			}
		}
	}
}

void DfxFluentTracker::clear() {
	_sceneNodeTracker.clear();
	_simplexNodeTracker.clear();
}