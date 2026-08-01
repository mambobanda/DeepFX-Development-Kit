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

#include "DfxQListenerItemOrderButton.h"

QSvgRenderer *DfxQListenerItemOrderButton::renderer_up = nullptr;
QSvgRenderer *DfxQListenerItemOrderButton::renderer_down = nullptr;

DfxQListenerItemOrderButton::DfxQListenerItemOrderButton(int upDown, QGraphicsItem *parent):QGraphicsPathItem(parent) {
	if (renderer_up == nullptr) renderer_up = new QSvgRenderer(QLatin1String(":/icon/listener_item_order_up"));
	if (renderer_down == nullptr) renderer_down = new QSvgRenderer(QLatin1String(":/icon/listener_item_order_down"));

	icon = new QGraphicsSvgItem(this);

	if(!upDown) icon->setSharedRenderer(renderer_down);
	else icon->setSharedRenderer(renderer_up);

	icon->setScale(0.022);
	//icon->setPos(3.5, 2.4);
}

DfxQListenerItemOrderButton::~DfxQListenerItemOrderButton() {

}

int DfxQListenerItemOrderButton::type() const {
	return Type;
}

void DfxQListenerItemOrderButton::draw() {
	QPainterPath p;

	p.addRect(0, 0, 11, 10);
	_box = p;

	setPath(p);
}

void DfxQListenerItemOrderButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(Qt::white, 2));
	painter->setBrush(QColor(255, 255, 255));

	painter->drawPath(_box);
}

void DfxQListenerItemOrderButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(0.4);
}

void DfxQListenerItemOrderButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(1.0);
	emit buttonClicked(_flowPath);
}

std::string DfxQListenerItemOrderButton::flowPath() {
	return _flowPath;
}

void DfxQListenerItemOrderButton::setFlowPath(std::string fpth) {
	_flowPath = fpth;
}