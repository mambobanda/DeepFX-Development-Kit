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
#include "DfxQFlowCollection.h"
#include "DfxQFlowProcessNode.h"
#include "DfxQFlowDecisionNode.h"
#include "DfxQFlowBreakNode.h"
#include "DfxQFlowStartNode.h"


DfxQFlowCollection::DfxQFlowCollection() {
	_router = new Avoid::Router(Avoid::OrthogonalRouting);
	_router->setRoutingPenalty(Avoid::segmentPenalty);
	_router->setRoutingParameter(Avoid::shapeBufferDistance, 20);
	//_router->setRoutingPenalty(Avoid::segmentPenalty);
	//_router->setRoutingPenalty(Avoid::crossingPenalty);
	//_router->setRoutingPenalty(Avoid::fixedSharedPathPenalty);
	//_router->setRoutingOption(Avoid::penaliseOrthogonalSharedPathsAtConnEnds, true);
	//_router->setRoutingOption(Avoid::improveHyperedgeRoutesMovingAddingAndDeletingJunctions, true);
	//_router->setRoutingOption(Avoid::nudgeOrthogonalSegmentsConnectedToShapes, true);
	//_router->setRoutingParameter(Avoid::idealNudgingDistance, 50);

}

DfxQFlowCollection::~DfxQFlowCollection() {
	delete _router;
}

void DfxQFlowCollection::drawBackground(QPainter *painter, const QRectF &rect) {

	int scale = 100000;
	int spacing = 10;

	painter->setPen(QPen(QColor(96, 96, 96, 50), 1.f));

	for (int x = -scale; x <= scale; x += spacing)
		painter->drawLine(x, -scale, x, scale);

	for (int y = -scale; y <= scale; y += spacing)
		painter->drawLine(-scale, y, scale, y);
}

std::string DfxQFlowCollection::flowPath() {
	return _flowPath;
}

void DfxQFlowCollection::setFlowPath(std::string pth) {
	_flowPath = pth;
}

void DfxQFlowCollection::setEditorPtr(DfxFluentNodeEditor *ptr) {
	_editor = ptr;
}

void DfxQFlowCollection::clearAll() {

	for (auto item : items()) {
		if (item->type() == DfxQNodeEnums::FLOW_START_NODE    ||
			item->type() == DfxQNodeEnums::FLOW_DECISION_NODE || 
			item->type() == DfxQNodeEnums::FLOW_PROCESS_NODE  || 
			item->type() == DfxQNodeEnums::FLOW_BREAK_NODE ) {

			auto _item = (DfxQFlowBaseNode*)item;
			_router->deleteShape(_item->avoidShape());
			removeItem(item);
		}
		else if(item->type() == DfxQNodeEnums::FLOW_CONNECTOR){

		}
		else {
			removeItem(item);
		}
	}

	_router->processTransaction();
	clear();
}

void DfxQFlowCollection::addExisting(std::string pth) {
	clearAll();

	setFlowPath(pth);

	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		auto flow = handler.flow();

		for (auto n : flow->nodes()) {
			restoreFlowNode(n->id());
		}

		restoreFlowLinks(flow);
	}
}

std::pair<std::string, unsigned int> DfxQFlowCollection::typeInfo() {
	return std::pair<std::string, unsigned int>(_flowPath, QFluentViewType::FLOW);
}

Avoid::Router *DfxQFlowCollection::router() {
	return _router;
}

void DfxQFlowCollection::addNewProcess() {
	DfxFlowHandler handler;
	
	if (handler.fetch(_flowPath)) {
		auto flow = handler.flow();

		auto d1 = flow->addProcess();
		flow->node(d1)->setText("Process Node");
		auto processNode = new DfxQFlowProcessNode(_router);
		processNode->setLabel("Process Node");
		processNode->setFid(d1);
		processNode->setFlowPath(_flowPath);

		addItem(processNode);
	}
}

void DfxQFlowCollection::addNewDecision() {
	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		auto flow = handler.flow();

		auto d1 = flow->addDecision();
		flow->node(d1)->setText("Decision Node Decision Node");
		auto decisionNode = new DfxQFlowDecisionNode(_router);
		decisionNode->setLabel("Decision Node Decision Node");
		decisionNode->setFid(d1);
		decisionNode->setFlowPath(_flowPath);

		addItem(decisionNode);
	}
}

void DfxQFlowCollection::addNewBreak() {
	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		auto flow = handler.flow();

		auto d1 = flow->addBreak();
		auto breakNode = new DfxQFlowBreakNode(_router);
		breakNode->setFid(d1);
		breakNode->setFlowPath(_flowPath);

		addItem(breakNode);
	}
}

void DfxQFlowCollection::restoreFlowNode(int fid) {
	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		auto flow  = handler.flow();
		auto fnode = flow->node(fid);
	
		if (fnode->type() == fl::Flow::START) {
			auto startNode = new DfxQFlowStartNode(_router);
			startNode->setFid(fid);
			startNode->setFlowPath(_flowPath);
			startNode->setPos(fnode->posX(), fnode->posY());

			addItem(startNode);
		}
		else if (fnode->type() == fl::Flow::DECISION) {
			auto decisionNode = new DfxQFlowDecisionNode(_router);
			decisionNode->setFid(fid);
			decisionNode->setFlowPath(_flowPath);
			decisionNode->setPos(fnode->posX(), fnode->posY());

			addItem(decisionNode);
		}
		else if (fnode->type() == fl::Flow::PROCESS) {
			auto processNode = new DfxQFlowProcessNode(_router);
			processNode->setFid(fid);
			processNode->setFlowPath(_flowPath);
			processNode->setPos(fnode->posX(), fnode->posY());

			addItem(processNode);
		}
		else if (fnode->type() == fl::Flow::BREAK) {
			auto breakNode = new DfxQFlowBreakNode(_router);
			breakNode->setFid(fid);
			breakNode->setFlowPath(_flowPath);
			breakNode->setPos(fnode->posX(), fnode->posY());

			addItem(breakNode);
		}


	}

}

void DfxQFlowCollection::restoreFlowLinks(fl::Flow *flow) {
	for (auto item : items()) {
		if (item->type() == DfxQNodeEnums::FLOW_START_NODE ||
			item->type() == DfxQNodeEnums::FLOW_PROCESS_NODE) {
			auto bnode = (DfxQFlowBaseNode*)item;
			DfxQFlowBaseNode *dest = nullptr;

			if (flow->node(bnode->fid())->next() != -1) {

				dest = findNodeByFlowID(flow->node(bnode->fid())->next());

				if (dest != nullptr) {
					auto link = new DfxQFlowLink(router());
					link->setSrcState(DfxQFlowConnector::DIRECT);

					link->setNode00(bnode);
					link->setNode01(dest);

					addItem(link);
				}
			}
		}
		else if (item->type() == DfxQNodeEnums::FLOW_DECISION_NODE) {
			auto bnode = (DfxQFlowBaseNode*)item;
			DfxQFlowBaseNode *dest00 = nullptr;
			DfxQFlowBaseNode *dest01 = nullptr;

			if (flow->node(bnode->fid())->yes() != -1) {
				dest00 = findNodeByFlowID(flow->node(bnode->fid())->yes());

				if (dest00 != nullptr) {
					auto link = new DfxQFlowLink(router());
					link->setSrcState(DfxQFlowConnector::YES);

					link->setNode00(bnode);
					link->setNode01(dest00);

					addItem(link);
				}
			}

			if (flow->node(bnode->fid())->no() != -1) {
				dest01 = findNodeByFlowID(flow->node(bnode->fid())->no());

				if (dest01 != nullptr) {
					auto link = new DfxQFlowLink(router());
					link->setSrcState(DfxQFlowConnector::NO);

					link->setNode00(bnode);
					link->setNode01(dest01);

					addItem(link);
				}
			}
		}
	}
}

DfxQFlowBaseNode *DfxQFlowCollection::findNodeByFlowID(int fid) {
	for (auto item00 : items()) {

		if (item00->type() == DfxQNodeEnums::FLOW_DECISION_NODE ||
			item00->type() == DfxQNodeEnums::FLOW_PROCESS_NODE ||
			item00->type() == DfxQNodeEnums::FLOW_BREAK_NODE) {
			auto bnode00 = (DfxQFlowBaseNode*)item00;
			if (bnode00->fid() == fid) {
				return bnode00;
			}
		}
	}

	return nullptr;
}