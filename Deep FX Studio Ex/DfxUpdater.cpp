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

#include "DfxUpdater.h"
#include "DfxTimelineManager.h"
#include "DfxOpenGLCacheQueue.h"

DfxUpdater *DfxUpdater::_instance = nullptr;

DfxUpdater::DfxUpdater() {

}

DfxUpdater::~DfxUpdater() {

}

DfxUpdater *DfxUpdater::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxUpdater;
	}

	return _instance;
}

void DfxUpdater::sceneUpdated(unsigned int updateType, bool updateKeys) {

	if (DfxMode::getInstance()->animationMode() != DfxMode::AnimationMode::SIMULATION) {
		std::vector<unsigned int> ignore;

		if (!updateKeys) ignore.push_back(DfxMainEventUpdateID::KEYFRAME_WRANGLER_UPDATE);

		DfxMainEventLoop::getInstance()->processEvents(DfxTimelineManager::getInstance()->currentFrame(), updateType, ignore);
		DfxMainEventLoop::getInstance()->emitfinshedProcessingEventsSignal();
	}

	deepsg::SgTraversalFunction updateVertexBuffer = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

			DfxObjectHandler handler;
			auto geoNode = (deepsg::SgGeoNode*)node;

			if (handler.fetch(node->referencePath())) {

				if (handler.updateCache()) {
					
					auto cache = handler.objectCache();
					if (cache != nullptr) {
						DfxOpenGLCacheQueue::getInstance()->queueCache(cache, DfxOpenGLCacheQueue::VERTEX_BUFFER_CACHE);
					}	

					handler.setUpdateCache(false);
				}
			}
		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(updateVertexBuffer);

	emit _sceneUpdated();
}

void DfxUpdater::nodeTransformUpdated(std::vector<deepsg::SgNode*> nodes) {
	emit _nodeTransformUpdated(nodes);
}

void DfxUpdater::materialUpdated() {
	emit _materialUpdated();
}