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

#include <SgToolNode.h>
#include "DfxMainEventLoop.h"
#include "DfxToolsMainEventUpdate.h"

DfxToolsMainEventUpdate::DfxToolsMainEventUpdate() :DfxMainEventProcess(DfxMainEventUpdateID::TOOL_NODE_UPDATE) {

}

DfxToolsMainEventUpdate::~DfxToolsMainEventUpdate() {

}

void DfxToolsMainEventUpdate::process(int frame, unsigned int updateType) {

	deepsg::SgTraversalFunction evaluateTools = [updateType](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		for (auto child : node->children()) {
			if (child->getType() == deepsg::SgNodeType::TOOL) {
				auto tool = (deepsg::SgToolNode*)child;
				if (tool->isReady()) {

					if (updateType == DfxMainEventLoop::VIEW_UPDATE) {
						if (tool->evaluateOnUpdate()) {
							tool->evaluate(node);
						}
					}
					else if (updateType == DfxMainEventLoop::KEY_FRAME_UPDATE) {
						if (tool->evaluateOnNewFrame()) {
							tool->evaluate(node);
						}
					}
				}
			}
		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(evaluateTools);
}