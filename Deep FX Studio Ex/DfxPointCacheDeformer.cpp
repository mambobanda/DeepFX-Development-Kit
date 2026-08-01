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

#include "DfxPointCacheDeformer.h"


DfxPointCacheDeformer::DfxPointCacheDeformer(DfxPointCache *cache) {
	_cache = cache;
	_enabled = true;
	_frame = nullptr;
}

DfxPointCacheDeformer::~DfxPointCacheDeformer() {

}

bool DfxPointCacheDeformer::isVisible() {
	return false;
}

bool DfxPointCacheDeformer::isEnabled() {
	return _enabled;
}

void DfxPointCacheDeformer::begin(void *meshPtr, size_t meshType) {
	int frame = DfxTimelineManager::getInstance()->currentFrame();
	if (_cache->hasFrame(frame)) _frame = _cache->getFrame(frame);
}

glm::vec3 DfxPointCacheDeformer::filter(glm::vec3 base_pos, glm::vec3 final_pos, size_t index, void *meshPtr, size_t meshType) {

	if (_frame != nullptr) {
		//QThread::msleep(100);
		if (index < _frame->size()) return _frame->getPositions()[index];
	}

	return base_pos;
}

void DfxPointCacheDeformer::end(void *meshPtr, size_t meshType) {
	_frame = nullptr;
}