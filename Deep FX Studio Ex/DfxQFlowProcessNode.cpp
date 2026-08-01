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

#include "DfxQFlowProcessNode.h"


DfxQFlowProcessNode::DfxQFlowProcessNode(Avoid::Router *rt, QGraphicsItem *parent): DfxQFlowBaseNode(rt, parent) {

	_connector = new DfxQFlowConnector(rt, this);
	_connector_input = new DfxQFlowConnector(rt, this);

	_connector->setParentNode(this);
	_connector_input->setParentNode(this);

	_connector_input->setState(DfxQFlowConnector::INPUT);

	_label = new QGraphicsTextItem("Process Node", this);
	_label->setDefaultTextColor(QColor(Qt::black));
	_label->setFont(labelFont());

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

DfxQFlowProcessNode::~DfxQFlowProcessNode() {

}

int DfxQFlowProcessNode::type() const {
	return Type;
}

QPointF DfxQFlowProcessNode::center() const {
	return boundingRect().center() + pos();
}

QString DfxQFlowProcessNode::label() {
	return _label->toPlainText();
}

void DfxQFlowProcessNode::setLabel(QString lb) {
	_label->setPlainText(lb);
}

void DfxQFlowProcessNode::draw() {

	QPainterPath path;
	path.addRect(-2.5, -4, _label->boundingRect().width() + 6, _label->boundingRect().height() + 9);
	_connector->setPos(((_label->boundingRect().width() * 0.5f) - 3), _label->boundingRect().height() + 13);
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

QFont DfxQFlowProcessNode::labelFont() {

	QFont font;
	font.setPointSize(9);
	font.setBold(true);

	return font;
}

void DfxQFlowProcessNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	for (auto lnk : _links) {
		//lnk->updatePosition();
	}

	DfxQFlowBaseNode::mouseMoveEvent(event);
}

void DfxQFlowProcessNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow, 2));
		painter->setBrush(QColor(255, 255, 255));
	}
	else {
		painter->setPen(QPen(QColor(61, 150, 226, 255), 1));
		painter->setBrush(QColor(255, 255, 255));
	}

	painter->drawPath(_shape);

}

QPointF DfxQFlowProcessNode::normalConnectorPos() {
	return _connector->scenePos() + QPointF(5, 5);
}

QPointF DfxQFlowProcessNode::inputConnectorPos() {
	return _connector_input->scenePos() + QPointF(5, 5);
}

void DfxQFlowProcessNode::removeFromFlow() {
	auto space = DfxSpace::getInstance();
	DfxFlowHandler handler;

	if (handler.fetch(flowPath())) {
		auto flow = handler.flow();
		
		for (auto f: flow->node(fid())->paths()) {
			auto any = space->findAbsolute(f);

			if (any != nullptr) space->remove(any);
		}

		flow->removeProcess(fid());
	}
}
