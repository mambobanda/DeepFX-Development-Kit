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

#include "DfxQListenerItemDeleteButton.h"
#include "DfxQListenerNode.h"

QSvgRenderer *DfxQListenerItemDeleteButton::renderer = nullptr;

DfxQListenerItemDeleteButton::DfxQListenerItemDeleteButton(float radius, QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	_radius = 25.5;

	if (renderer == nullptr) renderer = new QSvgRenderer(QLatin1String(":/icon/listener_item_del"));
	icon = new QGraphicsSvgItem(this);

	icon->setSharedRenderer(renderer);
	icon->setScale(0.04);
	icon->setPos(1.8, 2);

	_flowPath = "";
}

DfxQListenerItemDeleteButton::~DfxQListenerItemDeleteButton() {

}

int DfxQListenerItemDeleteButton::type() const {
	return Type;
}

void DfxQListenerItemDeleteButton::draw() {
	QPainterPath p;

	p.addEllipse(0, 0, _radius, _radius);
	_circle = p;

	setPath(p);
}

void DfxQListenerItemDeleteButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(Qt::white, 2));
	painter->setBrush(QColor(255, 255, 255));

	painter->drawPath(_circle);
}

void DfxQListenerItemDeleteButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(0.5);
}

void DfxQListenerItemDeleteButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(1.0);

	DfxFlowHandler handler;

	if (handler.fetch(_flowPath)) {
		node()->deleteItem(handler);
	}
}

DfxQListenerNode *DfxQListenerItemDeleteButton::node() {
	return _node;
}

void DfxQListenerItemDeleteButton::setNode(DfxQListenerNode *pnt) {
	_node = pnt;
}


std::string DfxQListenerItemDeleteButton::flowPath() {
	return _flowPath;
}

void DfxQListenerItemDeleteButton::setFlowPath(std::string pth) {
	_flowPath = pth;
}