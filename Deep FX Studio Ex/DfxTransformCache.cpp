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


#include "DfxTransformCache.h"

DfxTransformFrame::DfxTransformFrame() {
	cached = false;
	_xform = glm::mat4(1.f);
}

DfxTransformFrame::DfxTransformFrame(glm::mat4 mt) {
	cached = false;
	_xform = mt;
}

bool DfxTransformFrame::isCached() {
	return cached;
}

void DfxTransformFrame::setIsCached(bool cch) {
	cached = cch;
}

void DfxTransformFrame::clear() {
	cached = false;
	_xform = glm::mat4(1.f);
}

void DfxTransformFrame::setXForm(glm::mat4 mt) {
	_xform = mt;
}

glm::mat4 DfxTransformFrame::xform() {
	return _xform;
}

DfxTransformCache::DfxTransformCache() : DfxSimulationCacheBase(DfxSimulationCacheBase::TRANSFORM_CACHE) {
}

DfxTransformCache::~DfxTransformCache() {
	clear();
}

void DfxTransformCache::insertFrame(int frame, DfxTransformFrame *sframe) {

	std::pair<std::string, DfxTransformFrame*> p(utils::indexToKey(frame), sframe);
	frames.insert(p);
}

DfxTransformFrame* DfxTransformCache::getFrame(int index) {

	std::unordered_map<std::string, DfxTransformFrame*>::iterator fr = frames.find(utils::indexToKey(index));

	if (fr == frames.end()) {
		return nullptr;
	}
	else {
		DfxTransformFrame* ptr = fr->second;
		return ptr;
	}

}

bool DfxTransformCache::hasFrame(int frame) {
	std::unordered_map<std::string, DfxTransformFrame*>::iterator fr = frames.find(utils::indexToKey(frame));

	if (fr != frames.end()) {
		return true;
	}

	return false;
}

void DfxTransformCache::clear() {
	std::unordered_map<std::string, DfxTransformFrame*>::iterator it = frames.begin();

	for (; it != frames.end(); it++) {
		it->second->clear();
		delete it->second;
		it->second = nullptr;
	}

	frames.clear();
}