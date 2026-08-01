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

#include "DfxQFlowDecisionNode.h"


DfxQFlowDecisionNode::DfxQFlowDecisionNode(Avoid::Router *rt, QGraphicsItem *parent) : DfxQFlowBaseNode(rt, parent) {

	_connector_a = new DfxQFlowConnector(rt,this);
	_connector_b = new DfxQFlowConnector(rt,this);
	_connector_input = new DfxQFlowConnector(rt, this);

	_connector_a->setLabelText("YES");
	_connector_a->setLabelPos(DfxQFlowConnector::LEFT);
	_connector_a->setParentNode(this);
	_connector_a->setState(DfxQFlowConnector::YES);

	_connector_b->setLabelText("NO");
	_connector_b->setLabelPos(DfxQFlowConnector::RIGHT);
	_connector_b->setParentNode(this);
	_connector_b->setState(DfxQFlowConnector::NO);

	_connector_input->setParentNode(this);
	_connector_input->setState(DfxQFlowConnector::INPUT);

	_label = new QGraphicsTextItem("Decision Node Decision Node", this);
	_label->setDefaultTextColor(QColor(Qt::black));
	//_label->setTextWidth(150);
	_label->setFont(labelFont());

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

}

DfxQFlowDecisionNode::~DfxQFlowDecisionNode() {

}

int DfxQFlowDecisionNode::type() const {
	return Type;
}

QPointF DfxQFlowDecisionNode::center() const {
	return boundingRect().center() + pos();
}

QString DfxQFlowDecisionNode::label() {
	return _label->toPlainText();
}

void DfxQFlowDecisionNode::setLabel(QString lb) {
	_label->setPlainText(lb);
}

void DfxQFlowDecisionNode::draw() {

	QPainterPath path;
	QPainterPath path01;
	QPainterPath path02;
	QPainterPath path03;

	QFontMetrics fm(labelFont());

	float length = (fm.width(_label->toPlainText()) / 1.41421356f) + 6;
	
	QPolygonF polygon00;
	polygon00.append(QPointF(0.0f, 0.0f));
	polygon00.append(QPointF((_label->boundingRect().width() + 1) * 0.5f, -30.0f));
	polygon00.append(QPointF(_label->boundingRect().width(), 0.0f));
	polygon00.append(QPointF(0.0f, 0.0f));
	path02.addPolygon(polygon00);

	QPolygonF polygon01;
	polygon01.append(QPointF(0.0f, _label->boundingRect().height()));
	polygon01.append(QPointF((_label->boundingRect().width() + 1) * 0.5f, _label->boundingRect().height() + 30.0f));
	polygon01.append(QPointF(_label->boundingRect().width(), _label->boundingRect().height()));
	polygon01.append(QPointF(0.0f, _label->boundingRect().height()));
	path02.addPolygon(polygon01);

	path01.addRect(-2.5, -4, _label->boundingRect().width() + 6, _label->boundingRect().height() + 9);
	path = path02.united(path01);
	path = path.united(path03);

	_connector_a->setPos(-20, (_label->boundingRect().height() - 10) * 0.5);
	_connector_b->setPos(_label->boundingRect().width() + 12, (_label->boundingRect().height() - 10) * 0.5);
	_connector_input->setPos((_label->boundingRect().width() * 0.5f) - 4.f, -_label->boundingRect().height() - 25.0f);

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

QFont DfxQFlowDecisionNode::labelFont() {

	QFont font;
	font.setPointSize(9);
	font.setBold(true);

	return font;
}

void DfxQFlowDecisionNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	
	for (auto lnk : _links) {
		// lnk->updatePosition();
	}

	DfxQFlowBaseNode::mouseMoveEvent(event);
}

void DfxQFlowDecisionNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

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

QPointF DfxQFlowDecisionNode::yesConnectorPos() {
	return _connector_a->scenePos() + QPointF(5, 5);
}

QPointF DfxQFlowDecisionNode::noConnectorPos() {
	return _connector_b->scenePos() + QPointF(5, 5);
}

QPointF DfxQFlowDecisionNode::inputConnectorPos() {
	return _connector_input->scenePos() + QPointF(5, 5);
}

void DfxQFlowDecisionNode::removeFromFlow() {
	auto space = DfxSpace::getInstance();
	DfxFlowHandler handler;

	if (handler.fetch(flowPath())) {
		auto flow = handler.flow();

		if (!flow->node(fid())->conditionPath().empty()) {
			auto any = space->findAbsolute(flow->node(fid())->conditionPath());
			space->remove(any);
		}

		flow->removeDecision(fid());
	}
}