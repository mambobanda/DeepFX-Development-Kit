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

#include "DfxQFlowLink.h"
#include "DfxQFlowBaseNode.h"
#include "DfxQFlowConnector.h"


size_t DfxQFlowLink::ID_GEN = 0;

DfxQFlowLink::DfxQFlowLink(Avoid::Router *rt, QGraphicsItem *parent) : QGraphicsPathItem(parent) {

	_uid = ID_GEN++;

	_node00 = nullptr;
	_node01 = nullptr;

	setZValue(-1);

	_router = rt;

	Avoid::Point srcPt(0, 0);
	Avoid::Point dstPt(1, 1);
	_connRef = new Avoid::ConnRef(_router, srcPt, dstPt);

	_srcState = DfxQFlowConnector::DIRECT;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

DfxQFlowLink::~DfxQFlowLink() {

	if (_node00 != nullptr) {
		_node00->removeLink(this);
	}

	if (_node01 != nullptr) {
		_node01->removeLink(this);

		_node00 = nullptr;
	}

	//DfxGlobal::console::println("Killing Link");
}

size_t DfxQFlowLink::uid() {
	return _uid;
}

DfxQFlowBaseNode *DfxQFlowLink::node00() {
	return _node00;
}

void DfxQFlowLink::setNode00(DfxQFlowBaseNode *slt) {
	_node00 = slt;
	_node00->addLink(this);
}

DfxQFlowBaseNode *DfxQFlowLink::node01() {
	return _node01;
}

void DfxQFlowLink::setNode01(DfxQFlowBaseNode *slt) {
	_node01 = slt;
	_node01->addLink(this);
}

void DfxQFlowLink::updatePosition(QPointF a, QPointF b) {

	QPainterPath p;

	auto pos1 = a;
	auto pos2 = b;

	p.moveTo(pos1);

	qreal dx = pos2.x() - pos1.x();
	qreal dy = pos2.y() - pos1.y();

	QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
	QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

	p.cubicTo(ctr1, ctr2, pos2);

	setPath(p);

}

void DfxQFlowLink::updatePosition() {

	if (_node00 != nullptr && _node01 != nullptr) {
		QPainterPath p;

		QPointF pos1;
		QPointF pos2;

		if (_srcState == DfxQFlowConnector::DIRECT) {
			pos1 = _node00->normalConnectorPos();
		}
		else if (_srcState == DfxQFlowConnector::YES) {
			pos1 = _node00->yesConnectorPos();
		}
		else if (_srcState == DfxQFlowConnector::NO) {
			pos1 = _node00->noConnectorPos();
		}

		pos2 = _node01->inputConnectorPos();

		Avoid::Point srcPt(pos1.x(), pos1.y());
		Avoid::Point dstPt(pos2.x(), pos2.y());

		_connRef->setSourceEndpoint(srcPt);
		_connRef->setDestEndpoint(dstPt);

		_router->processTransaction();
		
		auto route = _connRef->displayRoute();

		if (route.size() > 1) {
			for (size_t i = 1; i < route.size(); ++i)
			{
				Avoid::Point pointA = route.at(i - 1);
				Avoid::Point pointB = route.at(i);
				
				p.moveTo(pointA.x, pointA.y);
				p.lineTo(pointB.x, pointB.y);
			}
		}

		/*p.moveTo(pos1);

		qreal dx = pos2.x() - pos1.x();
		qreal dy = pos2.y() - pos1.y();

		QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
		QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

		p.cubicTo(ctr1, ctr2, pos2);*/

		setPath(p);
	}
}

void DfxQFlowLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHint(QPainter::Antialiasing);

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow, 2));
		painter->setBrush(Qt::NoBrush);
	}
	else {
		painter->setPen(QPen(Qt::white, 2));
		painter->setBrush(Qt::NoBrush);
	}

	painter->drawPath(path());
}

int DfxQFlowLink::type() const {
	return Type;
}

uint DfxQFlowLink::srcState() {
	return _srcState;
}

void DfxQFlowLink::setSrcState(uint st) {
	_srcState = st;
}

void DfxQFlowLink::bind() {
	DfxFlowHandler handler;

	if (handler.fetch(node00()->flowPath())) {
		auto flow = handler.flow();

		if(srcState() == DfxQFlowConnector::DIRECT)
			flow->node(node00()->fid())->setNext(node01()->fid());
		else if(srcState() == DfxQFlowConnector::YES)
			flow->node(node00()->fid())->setYes(node01()->fid());
		else if (srcState() == DfxQFlowConnector::NO)
			flow->node(node00()->fid())->setNo(node01()->fid());
	}
}

void DfxQFlowLink::unbind() {
	DfxFlowHandler handler;

	if (handler.fetch(node00()->flowPath())) {
		auto flow = handler.flow();

		if (srcState() == DfxQFlowConnector::DIRECT)
			flow->node(node00()->fid())->setNext(-1);
		else if (srcState() == DfxQFlowConnector::YES)
			flow->node(node00()->fid())->setYes(-1);
		else if (srcState() == DfxQFlowConnector::NO)
			flow->node(node00()->fid())->setNo(-1);
	}
}

DfxQFlowLink *DfxQFlowLink::findExistingLink(uint srcState, DfxQFlowBaseNode *srcNode) {
	DfxFlowHandler handler;

	if (handler.fetch(srcNode->flowPath())) {
		auto flow = handler.flow();

		if (srcState == DfxQFlowConnector::DIRECT) {
			if(flow->node(srcNode->fid())->next() != -1) return srcNode->findLinkWithSrc(srcNode, srcState);
		}
		else if (srcState == DfxQFlowConnector::YES) {
			if(flow->node(srcNode->fid())->yes() != -1)	return srcNode->findLinkWithSrc(srcNode, srcState);
		}
		else if (srcState == DfxQFlowConnector::NO) {
			if(flow->node(srcNode->fid())->no()  != -1) return srcNode->findLinkWithSrc(srcNode, srcState);
		}
	}

	return nullptr;
}