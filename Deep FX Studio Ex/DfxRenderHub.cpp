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

#include "DfxRenderHub.h"

DfxRenderHub *DfxRenderHub::_instance = nullptr;

DfxRenderHub::DfxRenderHub() {

}

DfxRenderHub::~DfxRenderHub() {

}

DfxRenderHub *DfxRenderHub::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxRenderHub;
	}

	return _instance;
}

void DfxRenderHub::addRenderNode(DfxRenderNodeBase *rnd) {
	_renderNode.push_back(rnd);
	emit renderNodesUpdated();
}

DfxRenderNodeBase *DfxRenderHub::renderNode(size_t inx) {
	return _renderNode[inx];
}

void DfxRenderHub::removeRenderNode(DfxRenderNodeBase *rnd) {
	auto itr = _renderNode.begin();

	while (itr != _renderNode.end()) {
		if ((*itr)->getID() == rnd->getID()) {
			_renderNode.erase(itr);
			emit renderNodesUpdated();
			return;
		}
		itr++;
	}
}

int DfxRenderHub::renderNodeCount() {
	return _renderNode.size();
}
