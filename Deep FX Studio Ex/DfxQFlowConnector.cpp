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

#include "DfxQFlowConnector.h"

DfxQFlowConnector::DfxQFlowConnector(Avoid::Router *rt, QGraphicsItem *parent) : DfxQFlowBaseNode(rt, parent) {

	_label = new QGraphicsTextItem("", this);
	_labelPos = LEFT;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	_state = DIRECT;
}

DfxQFlowConnector::~DfxQFlowConnector() {

}

int DfxQFlowConnector::type() const {
	return Type;
}

QString DfxQFlowConnector::labelText() {
	return _label->toPlainText();
}

void DfxQFlowConnector::setLabelText(QString str) {
	_label->setPlainText(str);
}

uint DfxQFlowConnector::labelPos() {
	return _labelPos;
}

void DfxQFlowConnector::setLabelPos(uint ps) {
	_labelPos = ps;
}

uint DfxQFlowConnector::state() {
	return _state;
}

void DfxQFlowConnector::setState(uint st) {
	_state = st;
}

QPointF DfxQFlowConnector::center() const {
	return boundingRect().center() + pos();
}

void DfxQFlowConnector::draw() {
	QPainterPath path;
	path.addEllipse(0, 0, 10, 10);

	if (_labelPos == LEFT) {
		_label->setPos(-25, -5);
	}
	else {
		_label->setPos(10, -5);
	}

	_shape = path;

	setPath(path);
}

void DfxQFlowConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(QColor(255, 153, 85, 0), 0));

	if (state() == INPUT) {
		painter->setBrush(QColor(255, 153, 83, 255));
	}
	else {
		painter->setBrush(QColor(61, 150, 226, 255));
	}

	painter->drawPath(_shape);

}

//void DfxQFlowConnector::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
//
//}

DfxQFlowBaseNode *DfxQFlowConnector::parentNode() {
	return _parentNode;
}

void DfxQFlowConnector::setParentNode(DfxQFlowBaseNode *prt) {
	_parentNode = prt;
}

void DfxQFlowConnector::removeFromFlow() {

}