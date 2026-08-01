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

#include "DfxObjectDisplacementMap.h"


DfxObjectDisplacement::DfxObjectDisplacement() {
	position = nullptr;
	cached = false;
	count = 0;
}

void DfxObjectDisplacement::clear() {
	delete[] position;
	count = 0;

	position = nullptr;
}

void DfxObjectDisplacement::init(size_t count) {
	this->count = count;
	position = new glm::vec3[count];
}

glm::vec3 *DfxObjectDisplacement::getPositions() {
	return position;
}

size_t DfxObjectDisplacement::getCount() {
	return count;
}

bool DfxObjectDisplacement::isCached() {
	return cached;
}

void DfxObjectDisplacement::setIsCached(bool ca) {
	cached = ca;
}

DfxObjectDisplacementMap::DfxObjectDisplacementMap() {

}

DfxObjectDisplacementMap::~DfxObjectDisplacementMap() {
	clear();
}

size_t DfxObjectDisplacementMap::getCount() {
	return displacements.size();
}

std::string DfxObjectDisplacementMap::indexToKey(size_t index) {

	std::stringstream ss;
	ss << index << "_frame";

	return ss.str();
}

void DfxObjectDisplacementMap::insertDisplacement(size_t frame, DfxObjectDisplacement *disp) {
	
	auto it = displacements.find(indexToKey(frame));

	if (it == displacements.end()) {
		std::pair<std::string, DfxObjectDisplacement*> p(indexToKey(frame), disp);
		displacements.insert(p);
	}
	else {
		delete it->second;
		it->second = disp;
	}
}

DfxObjectDisplacement* DfxObjectDisplacementMap::getDisplacement(size_t index) {

	std::unordered_map<std::string, DfxObjectDisplacement*>::iterator fr = displacements.find(indexToKey(index));

	if (fr == displacements.end()) {
		return nullptr;
	}
	else {
		DfxObjectDisplacement* ptr = fr->second;

		return ptr;
	}
}

void DfxObjectDisplacementMap::clear() {
	std::unordered_map<std::string, DfxObjectDisplacement*>::iterator it = displacements.begin();

	for (; it != displacements.end(); it++) {
		it->second->clear();
		delete it->second;
		it->second = nullptr;
	}

	displacements.clear();
}