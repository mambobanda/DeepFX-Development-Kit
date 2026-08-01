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

#include "DfxQComputePropertyLink.h"
#include "DfxQComputeNodeSlot.h"

size_t DfxQComputePropertyLink::ID_GEN = 0;

DfxQComputePropertyLink::DfxQComputePropertyLink(QGraphicsItem *parent) : QGraphicsPathItem(parent) {

	_uid = ID_GEN++;

	_slot00 = nullptr;
	_slot01 = nullptr;

	setZValue(-1);

	_linkType = NORMAL;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

DfxQComputePropertyLink::~DfxQComputePropertyLink() {
	if (_slot00 != nullptr) {
		_slot00->removeLink(this);
	}

	if (_slot01 != nullptr) {
		_slot01->removeLink(this);

		_slot00 = nullptr;
		_slot00 = nullptr;
	}

	//DfxGlobal::console::println("Killing Link");
}

size_t DfxQComputePropertyLink::uid() {
	return _uid;
}

DfxQComputeNodeSlot *DfxQComputePropertyLink::slot00() {
	return _slot00;
}

void DfxQComputePropertyLink::setSlot00(DfxQComputeNodeSlot *slt) {
	_slot00 = slt;
	_slot00->addLink(this);
}

DfxQComputeNodeSlot *DfxQComputePropertyLink::slot01() {
	return _slot01;
}

void DfxQComputePropertyLink::setSlot01(DfxQComputeNodeSlot *slt) {
	_slot01 = slt;
	_slot01->addLink(this);
}

void DfxQComputePropertyLink::updatePosition(QPointF a, QPointF b) {

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

void DfxQComputePropertyLink::updatePosition() {

	if (_slot00 != nullptr && _slot01 != nullptr) {
		QPainterPath p;

		auto pos1 = _slot00->scenePos();
		auto pos2 = _slot01->scenePos();

		p.moveTo(pos1);

		qreal dx = pos2.x() - pos1.x();
		qreal dy = pos2.y() - pos1.y();

		QPointF ctr1(pos1.x() + dx * 0.25, pos1.y() + dy * 0.1);
		QPointF ctr2(pos1.x() + dx * 0.75, pos1.y() + dy * 0.9);

		p.cubicTo(ctr1, ctr2, pos2);

		setPath(p);
	}
}

void DfxQComputePropertyLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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

int DfxQComputePropertyLink::type() const {
	return Type;
}

uint DfxQComputePropertyLink::linkType() {
	return _linkType;
}

void DfxQComputePropertyLink::setLinkType(uint lt) {
	_linkType = lt;
}