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

#include "DfxQListenerItemEditButton.h"
#include "DfxQFluentNodeEditor.h"
#include "DfxQListenerItem.h"

QSvgRenderer *DfxQListenerItemEditButton::renderer = nullptr;

DfxQListenerItemEditButton::DfxQListenerItemEditButton(float radius, QGraphicsItem *parent):QGraphicsPathItem(parent) {
	_radius = 25.5;

	if(renderer == nullptr) renderer = new QSvgRenderer(QLatin1String(":/icon/listener_item_edit"));
	icon = new QGraphicsSvgItem(this);

	_item = nullptr;
	_editor = nullptr;

	icon->setSharedRenderer(renderer);
	icon->setScale(0.045);
	icon->setPos(3.6, 2.4);
}

DfxQListenerItemEditButton::~DfxQListenerItemEditButton() {

}

int DfxQListenerItemEditButton::type() const {
	return Type;
}

void DfxQListenerItemEditButton::setItemPtr(DfxQListenerItem *ptr) {
	_item = ptr;
}

void DfxQListenerItemEditButton::draw() {
	QPainterPath p;

	p.addEllipse(0, 0, _radius, _radius);
	_circle = p;

	setPath(p);
}

void DfxQListenerItemEditButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(Qt::white, 2));
	painter->setBrush(QColor(255, 255, 255));

	painter->drawPath(_circle);
}

void DfxQListenerItemEditButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(0.5);
}

void DfxQListenerItemEditButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	icon->setOpacity(1.0);

	DfxQFluentEditorCrumb crumb;
	crumb.type = QFluentViewType::FLOW;
	crumb.path = _item->flowPath();
	_editor->pushView(crumb);
}

void DfxQListenerItemEditButton::setEditorPtr(DfxFluentNodeEditor *flt) {
	_editor = flt;
}