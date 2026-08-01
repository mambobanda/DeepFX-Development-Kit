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

#include <SgPointCloudNode.h>
#include "DfxBulletActorNode.h"
#include "DfxBulletPointCloudCacheFilter.h"

DfxBulletPointCloudCacheFilter::DfxBulletPointCloudCacheFilter() {
	_bulletActor = nullptr;
	xformOverride = nullptr;
	size = 0;
}

DfxBulletPointCloudCacheFilter::~DfxBulletPointCloudCacheFilter() {

}

DfxBulletActorNode *DfxBulletPointCloudCacheFilter::bulletActor() {
	return _bulletActor;
}

void DfxBulletPointCloudCacheFilter::setBulletActor(DfxBulletActorNode *bta) {
	_bulletActor = bta;
}

void DfxBulletPointCloudCacheFilter::begin(deepsg::SgNode *nd, deepsg::SgStateInfo *info) {

	if (nd->getType() == deepsg::SgNodeType::POINT_CLOUD) {
		auto pcloud = ((deepsg::SgPointCloudNode*)nd)->geo();

		size = pcloud->size();

		xformOverride = pcloud->channel("@transformOverride");
		if (xformOverride != nullptr) if (xformOverride->cType() != ds::type::MAT4) xformOverride = nullptr;
	}
}

void DfxBulletPointCloudCacheFilter::end(deepsg::SgNode *nd, deepsg::SgStateInfo *info) {
	xformOverride = nullptr;
	size = 0;
}

void DfxBulletPointCloudCacheFilter::filter(deepsg::SgNode *node, deepsg::SgStateInfo *info) {
	
	int frame = DfxTimelineManager::getInstance()->currentFrame();

	if (xformOverride != nullptr && _bulletActor != nullptr) {
		if (size > 0) {
			for (int inx = 0; inx < _bulletActor->agents().size(); inx++) {
				auto agent = _bulletActor->agents()[inx];
				if (agent->pointID < _bulletActor->agents().size()) {
					if (agent->cache != nullptr) {
						if (agent->cache->hasFrame(frame)) {

							if (agent->cache->type() == DfxSimulationCacheBase::TRANSFORM_CACHE) {

								auto mat = ((DfxTransformCache*)agent->cache)->getFrame(frame)->xform();
								xformOverride->at<glm::mat4>()[agent->pointID] = mat;
							}
						}
					}
				}
			}
		}
	}
}