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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <queue>
#include "DfxObjectCache.h"
#include "DfxVolumeCache.h"

class DfxOpenGLCacheQueue {
private:
	static DfxOpenGLCacheQueue *_instance;

	std::queue<DfxObjectCache*> _objectQueue;
	std::queue<DfxObjectCache*> _vertexQueue;
	std::queue<DfxVolumeCache*> _volumeQueue;

	DfxOpenGLCacheQueue();

public:
	enum { FULL_CACHE = 0, VERTEX_BUFFER_CACHE = 1 };

	virtual ~DfxOpenGLCacheQueue();

	static DfxOpenGLCacheQueue *getInstance();

	void queueCache(DfxObjectCache* cache, size_t extent = FULL_CACHE);
	void queueCache(DfxVolumeCache* cache);
	void cacheAndClear();
	void clear();
};