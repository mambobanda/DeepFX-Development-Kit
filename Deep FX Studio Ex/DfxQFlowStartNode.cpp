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

#include "DfxQFlowStartNode.h"


DfxQFlowStartNode::DfxQFlowStartNode(Avoid::Router *rt, QGraphicsItem *parent) : DfxQFlowBaseNode(rt, parent) {
	_connector = new DfxQFlowConnector(rt, this);
	_connector->setParentNode(this);

	_label = new QGraphicsTextItem("Start", this);
	_label->setDefaultTextColor(QColor(Qt::white));
	_label->setFont(labelFont());

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

DfxQFlowStartNode::~DfxQFlowStartNode() {

}

int DfxQFlowStartNode::type() const {
	return Type;
}

QPointF DfxQFlowStartNode::center() const {
	return boundingRect().center() + pos();
}

QString DfxQFlowStartNode::label() {
	return _label->toPlainText();
}

void DfxQFlowStartNode::setLabel(QString lb) {
	_label->setPlainText(lb);
}

void DfxQFlowStartNode::draw() {

	QPainterPath path;
	path.addEllipse(0, 0, 45, 45);
	_label->setPos(1.5, 12);
	_connector->setPos(17.5, 53);
	_shape = path;

	setPath(path);

	float cx = center().x();
	float cy = center().y();
	float p_width  = boundingRect().width();
	float p_height = boundingRect().height();

	Avoid::Rectangle rectangle(Avoid::Point(cx, cy), p_width, p_height);
	_shapeRef->setNewPoly(rectangle);
	_router->processTransaction();

	if (initialPaint) {
		for (auto lnk : _links) {
			lnk->updatePosition();
		}

		initialPaint = false;
	}
}

QFont DfxQFlowStartNode::labelFont() {

	QFont font;
	font.setPointSize(9);
	font.setBold(true);

	return font;
}

void DfxQFlowStartNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	for (auto lnk : _links) {
		//lnk->updatePosition();
	}

	DfxQFlowBaseNode::mouseMoveEvent(event);
}

void DfxQFlowStartNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow, 2));
		painter->setBrush(QColor(61, 150, 226, 255));
	}
	else {
		painter->setPen(QPen(QColor(61, 150, 226, 0), 0));
		painter->setBrush(QColor(61, 150, 226, 255));
	}

	painter->drawPath(_shape);


}

QPointF DfxQFlowStartNode::normalConnectorPos() {
	return _connector->scenePos() + QPointF(5, 5);
}

void DfxQFlowStartNode::removeFromFlow() {}