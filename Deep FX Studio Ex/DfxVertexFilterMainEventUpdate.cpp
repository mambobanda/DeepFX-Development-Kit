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


#include "DfxVertexFilterMainEventUpdate.h"

DfxVertexFilterMainEventUpdate::DfxVertexFilterMainEventUpdate():DfxMainEventProcess(DfxMainEventUpdateID::VERTEX_FILTER_UPDATE){
}

DfxVertexFilterMainEventUpdate::~DfxVertexFilterMainEventUpdate() {

}

void DfxVertexFilterMainEventUpdate::process(int frame, unsigned int updateType) {

	deepsg::SgTraversalFunction updateVertexBuffer = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
			
			DfxObjectHandler handler;
		
			if (handler.fetch(node->referencePath())) {

				auto geoNode = (deepsg::SgGeoNode*)node;

				handler.setUpdateCache(false);
				if (geoNode->isVertexBufferDirty()) {

					geoNode->copyBaseToVertexBuffer();
					geoNode->disableDirtyVertexBufferFlag();

					handler.objectCache()->setVertexBufferUpdate(nullptr, 0);
					handler.setUpdateCache(true);

				}

				geoNode->evaluateVertexFilters();
				if (geoNode->hasVertexBufferUpdate()) {
					handler.setUpdateCache(true);
					handler.objectCache()->setVertexBufferUpdate(geoNode->vertexBuffer(), geoNode->vertexBufferSize());
					geoNode->disableVertexBufferUpdateFlag();
					geoNode->enableDirtyVertexBufferFlag();
				}

			/*	if (handler.updateCache()) {

					auto cache = handler.objectCache();
					if (cache != nullptr) {
						cache->cacheInterleavedPointsVBO(geoNode->vertexBuffer());
					}

					handler.setUpdateCache(false);
				}*/
				
			}
		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(updateVertexBuffer);
}