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

#include "DfxTimelineManager.h"
#include "DfxPointCache.h"
#include "DfxVertexFilter.h"

class DfxPointCacheDeformer : public DfxVertexFilter {
private:
	DfxPointCache *_cache;
	DfxPointFrame *_frame;

	bool _enabled;
	bool _visible;
public:
	DfxPointCacheDeformer(DfxPointCache *cache);
	virtual ~DfxPointCacheDeformer();

	bool isVisible() override;
	bool isEnabled() override;

	void begin(void *meshPtr = nullptr, size_t meshType = 0) override;
	void end(void *meshPtr = nullptr, size_t meshType = 0) override;

	glm::vec3 filter(glm::vec3 base_pos, glm::vec3 final_pos, size_t index, void *meshPtr = nullptr, size_t meshType = 0) override;
};