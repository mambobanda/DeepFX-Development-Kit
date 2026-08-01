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

#include "DfxSimulationCacheBase.h"
#include "DfxBulletBodyBase.h"
#include "DfxTransformCacheOverride.h"
#include "DfxBulletPointCloudCacheFilter.h"
#include "DfxSimulationCacheCommon.h"
#include "DfxTransformIdentityOverride.h"
#include "DfxBulletFluentBody.h"

class DfxBulletAgent {
public:
	DfxBulletAgent();

	void reset();
	
	bool ready;
	bool updateTransformCache;
	bool updatePointCache;
	bool kinematic;
	bool deforming;
	bool rigidBody;
	bool isStatic;
	bool fluent;
	btTriangleMesh *btTrimesh;
	///std::string fluentSet;
	size_t fluentUID;
	btTransform worldTransform;
	glm::vec3 scale;
	ds::PointCloud *pcloud;
	size_t pointID;
	DfxBulletBodyBase *body;
	DfxBulletFluentBody *fluentBody;
	DfxSimulationCacheBase *cache;
	deepsg::SgTransformOverride *transformOverride;
	DfxBulletPointCloudCacheFilter *pointCloudCacheFilter;
	deepsg::SgVertexFilter *pointCacheDeformer;
};