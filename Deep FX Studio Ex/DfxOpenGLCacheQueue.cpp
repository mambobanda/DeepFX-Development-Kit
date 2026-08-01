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


#include "DfxOpenGLCacheQueue.h"


DfxOpenGLCacheQueue *DfxOpenGLCacheQueue::_instance = nullptr;

DfxOpenGLCacheQueue::DfxOpenGLCacheQueue() {

}

DfxOpenGLCacheQueue::~DfxOpenGLCacheQueue() {

}

DfxOpenGLCacheQueue *DfxOpenGLCacheQueue::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxOpenGLCacheQueue;
	}

	return _instance;
}

void DfxOpenGLCacheQueue::queueCache(DfxObjectCache* cache, size_t extent) {
	if (extent == FULL_CACHE) {
		_objectQueue.push(cache);
	}
	else if (extent == VERTEX_BUFFER_CACHE) {
		_vertexQueue.push(cache);
	}
}

void DfxOpenGLCacheQueue::queueCache(DfxVolumeCache* cache) {
	_volumeQueue.push(cache);
}

void DfxOpenGLCacheQueue::cacheAndClear() {

	while (!_objectQueue.empty()) {
		_objectQueue.front()->cacheMesh();
		_objectQueue.pop();
	}

	while (!_vertexQueue.empty()) {
		_vertexQueue.front()->cacheInterleavedPointsVBO();
		_vertexQueue.pop();
	}

	while (!_volumeQueue.empty()) {
		_volumeQueue.front()->clear();
		_volumeQueue.front()->cacheVolume();
		_volumeQueue.pop();
	}
}

void DfxOpenGLCacheQueue::clear() {
	while (!_objectQueue.empty()) {
		_objectQueue.pop();
	}

	while (!_vertexQueue.empty()) {
		_vertexQueue.pop();
	}

	while (!_volumeQueue.empty()) {
		_volumeQueue.pop();
	}
}