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

#include "DfxQFlowBreakNode.h"
#include "DfxQFlowConnector.h"


DfxQFlowBreakNode::DfxQFlowBreakNode(Avoid::Router *rt, QGraphicsItem *parent) : DfxQFlowBaseNode(rt, parent) {

	_label = new QGraphicsTextItem("Break", this);
	_label->setDefaultTextColor(QColor(Qt::white));
	_label->setFont(labelFont());

	_connector_input = new DfxQFlowConnector(rt, this);
	_connector_input->setParentNode(this);
	_connector_input->setState(DfxQFlowConnector::INPUT);

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	initialPaint = true;

	/*Avoid::Rectangle rectangle(Avoid::Point(20.0, 35.0), Avoid::Point(40.0, 12.0));
	shapeRef = new Avoid::ShapeRef(router, rectangle);*/
}

DfxQFlowBreakNode::~DfxQFlowBreakNode() {

}

int DfxQFlowBreakNode::type() const {
	return Type;
}

QPointF DfxQFlowBreakNode::center() const {
	return boundingRect().center() + pos();
}

QString DfxQFlowBreakNode::label() {
	return _label->toPlainText();
}

void DfxQFlowBreakNode::setLabel(QString lb) {
	_label->setPlainText(lb);
}

void DfxQFlowBreakNode::draw() {

	QPainterPath path;
	path.addRoundedRect(-15, -4, _label->boundingRect().width() + 32, _label->boundingRect().height() + 9, 10, 10);
	_connector_input->setPos(((_label->boundingRect().width() * 0.5f) - 3), -_label->boundingRect().height());
	_shape = path;

	setPath(path);

	float cx = center().x();
	float cy = center().y();
	float p_width = boundingRect().width();
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

QFont DfxQFlowBreakNode::labelFont() {

	QFont font;
	font.setPointSize(9);
	font.setBold(true);

	return font;
}

void DfxQFlowBreakNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	
	for (auto lnk: _links) {
		//lnk->updatePosition();
	}

	DfxQFlowBaseNode::mouseMoveEvent(event);
}

void DfxQFlowBreakNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

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

QPointF DfxQFlowBreakNode::inputConnectorPos() {
	return _connector_input->scenePos() + QPointF(5, 5);
}

void DfxQFlowBreakNode::removeFromFlow() {
	DfxFlowHandler handler;

	if (handler.fetch(flowPath())) {
		auto flow = handler.flow();
		flow->removeBreak(fid());
	}
}