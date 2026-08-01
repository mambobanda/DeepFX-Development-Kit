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


#include "DfxTransformCacheOverride.h"

DfxTransformCacheOverride::DfxTransformCacheOverride(DfxTransformCache *cache) {
	cachePtr = cache;
}

DfxTransformCacheOverride::~DfxTransformCacheOverride() {
	
}

glm::mat4 DfxTransformCacheOverride::evaluate(deepsg::SgTransformOverrideInfo info) {
	if (cachePtr->hasFrame(info.frame)) {
		auto cacheFrame = cachePtr->getFrame(info.frame);
		return cacheFrame->xform();
	}

	return info.globalTransform;
}