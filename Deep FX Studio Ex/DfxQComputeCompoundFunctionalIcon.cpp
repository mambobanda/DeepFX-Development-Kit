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

#include "DfxQComputeCompoundFunctionalIcon.h"
#include "DfxQComputeCompoundNode.h"

QSvgRenderer *DfxQComputeCompoundFunctionalIcon::renderer = nullptr;
QSvgRenderer *DfxQComputeCompoundFunctionalIcon::renderer1 = nullptr;

DfxQComputeCompoundFunctionalIcon::DfxQComputeCompoundFunctionalIcon(QGraphicsItem *parent): QGraphicsPathItem(parent){

	if (renderer == nullptr) renderer = new QSvgRenderer(QLatin1String(":/icon/compound_functional_off_icon"));
	if (renderer1 == nullptr) renderer1 = new QSvgRenderer(QLatin1String(":/icon/compound_functional_on_icon"));

	icon = new QGraphicsSvgItem(this);

	_computeNodePtr = nullptr;

	icon->setSharedRenderer(renderer);
	icon->setScale(0.035);
}

DfxQComputeCompoundFunctionalIcon::~DfxQComputeCompoundFunctionalIcon() {

}

void DfxQComputeCompoundFunctionalIcon::setComputeNodePtr(DfxQComputeCompoundNode *cnode) {
	_computeNodePtr = cnode;
}

DfxQComputeCompoundNode *DfxQComputeCompoundFunctionalIcon::setComputeNodePtr() {
	return _computeNodePtr;
}

void DfxQComputeCompoundFunctionalIcon::setOffIcon() {


	icon->setSharedRenderer(renderer);
}

void DfxQComputeCompoundFunctionalIcon::setOnIcon() {
	icon->setSharedRenderer(renderer1);
}

int DfxQComputeCompoundFunctionalIcon::type() const {
	return Type;
}

void DfxQComputeCompoundFunctionalIcon::draw() {
	auto rect = icon->boundingRect();
	QPainterPath p;
	p.addRect(rect.x(), rect.y(), rect.width() * 0.035f, rect.height() * 0.035f);

	setPath(p);
}

void DfxQComputeCompoundFunctionalIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	draw();
}

void DfxQComputeCompoundFunctionalIcon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	if (_computeNodePtr->any()->rawData<fl::DataObject>()->isFunctional()) {
		_computeNodePtr->any()->rawData<fl::DataObject>()->setIsFunctional(false);
		setOffIcon();
	}
	else {
		_computeNodePtr->any()->rawData<fl::DataObject>()->setIsFunctional(true);
		setOnIcon();
	}
}