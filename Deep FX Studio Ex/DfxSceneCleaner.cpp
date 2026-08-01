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

#include "DfxSceneCleaner.h"
#include "DfxOpenGLCacheQueue.h"

DfxSceneCleaner *DfxSceneCleaner::_instance = nullptr;

DfxSceneCleaner::DfxSceneCleaner() {

}

DfxSceneCleaner::~DfxSceneCleaner() {

}

DfxSceneCleaner *DfxSceneCleaner::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxSceneCleaner;
	}

	return _instance;
}

int DfxSceneCleaner::addCallback(SceneCleanerCallback cb) {
	_cleanCallbackList.push_back(cb);
	return _cleanCallbackList.size() - 1;
}

void DfxSceneCleaner::removeCallback(int index) {
	_cleanCallbackList.erase(_cleanCallbackList.begin() + index);
}

void DfxSceneCleaner::clean() {
	for (auto cb : _cleanCallbackList) {
		cb();
	}
}

bool DfxSceneCleaner::cleanGraph(deepsg::SgNode *node) {

	std::vector<deepsg::SgNode*> _children;

	for (auto n: node->children()) _children.push_back(n);

	for (auto n : _children) {
		if (!cleanGraph(n)) return false;
	}

	if (node->getType() == deepsg::SgNodeType::ROOT) {
		return true;
	}
	else {
		auto delCallback = DfxSqNodeDB::getInstance()->deleteCallback(node->uniqueName());

		if (delCallback != nullptr) {
			delCallback(node);
			return true;
		}
		else {
			return false;
		}
	}
}

void DfxSceneCleaner::cleanGraph() {
	cleanGraph(deepsg::SgRootNode::getInstance());
	DfxOpenGLCacheQueue::getInstance()->clear();
}

void DfxSceneCleaner::reset() {
	emit resetSignal();
}