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

#include <SgCommon.h>
#include "DfxTimelineManager.h"
#include "DfxNodeFilter.h"
#include "DfxBulletCommon.h"

class DfxBulletPointCloudCacheFilter : public DfxNodeFilter {
private:
	DfxBulletActorNode *_bulletActor;
	ds::Channel *xformOverride;
	size_t size;
public:

	DfxBulletPointCloudCacheFilter();
	virtual ~DfxBulletPointCloudCacheFilter();

	DfxBulletActorNode *bulletActor();
	void setBulletActor(DfxBulletActorNode *);

	void begin(deepsg::SgNode *nd, deepsg::SgStateInfo *info) override;
	void end(deepsg::SgNode *nd, deepsg::SgStateInfo *info) override;

	void filter(deepsg::SgNode *node, deepsg::SgStateInfo *info) override;
};