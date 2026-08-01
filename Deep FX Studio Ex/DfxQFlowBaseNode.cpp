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

#include "DfxQFlowBaseNode.h"
#include "DfxFlowHandler.h"


size_t DfxQFlowBaseNode::UID_GEN = 0xff;

DfxQFlowBaseNode::DfxQFlowBaseNode(Avoid::Router *rt, QGraphicsItem *parent ): QGraphicsPathItem(parent) {
	_uid = UID_GEN++;
	_router = rt;

	Avoid::Rectangle rectangle(Avoid::Point(0, 0), 1, 1);
	_shapeRef = new Avoid::ShapeRef(_router, rectangle);
}

DfxQFlowBaseNode::~DfxQFlowBaseNode() {
	_router->deleteShape(_shapeRef);
}

size_t DfxQFlowBaseNode::uid() {
	return _uid;
}

int DfxQFlowBaseNode::fid() {
	return _fid;
}

void DfxQFlowBaseNode::setFid(int fi) {
	_fid = fi;
}

std::string DfxQFlowBaseNode::flowPath() {
	return _flowPath;
}

void DfxQFlowBaseNode::setFlowPath(std::string fpth) {
	_flowPath = fpth;
}

Avoid::ShapeRef *DfxQFlowBaseNode::avoidShape() {
	return _shapeRef;
}

void DfxQFlowBaseNode::addLink(DfxQFlowLink *lnk) {
	_links.push_back(lnk);
}

void DfxQFlowBaseNode::removeLink(DfxQFlowLink *lnk) {
	auto itr = _links.begin();
	
	while( itr != _links.end()) {
		if ((*itr)->uid() == lnk->uid()) {
			_links.erase(itr);
			break;
		}
		itr++;
	}
}

void DfxQFlowBaseNode::removeAndUnbindAllLinks() {
	std::vector<DfxQFlowLink*> tempArray;

	for (auto lk : _links) tempArray.push_back(lk);

	for (auto lk : tempArray) {
		lk->unbind();
		delete lk;
	}
}

void DfxQFlowBaseNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		auto flow = handler.flow();

		auto node = flow->node(_fid);

		if (node->type() != fl::Flow::GRAPH) {
			node->setPosX(pos().x());
			node->setPosY(pos().y());
		}
	}

	QGraphicsPathItem::mouseReleaseEvent(event);
}

QPointF DfxQFlowBaseNode::normalConnectorPos() {
	return QPointF(0.f, 0.f);
}

QPointF DfxQFlowBaseNode::yesConnectorPos() {
	return QPointF(0.f, 0.f);
}

QPointF DfxQFlowBaseNode::noConnectorPos() {
	return QPointF(0.f, 0.f);
}

QPointF DfxQFlowBaseNode::inputConnectorPos() {
	return QPointF(0.f, 0.f);
}

DfxQFlowLink *DfxQFlowBaseNode::findLinkWithSrc(DfxQFlowBaseNode *srcNode, uint srcState) {
	for (auto lk : _links) {
		if (lk->node00()->uid() == srcNode->uid() && lk->srcState() == srcState) return lk;
	}

	return nullptr;
}

void DfxQFlowBaseNode::updateLinkPositions() {
	for (auto lnk : _links) {
		lnk->updatePosition();
	}
}