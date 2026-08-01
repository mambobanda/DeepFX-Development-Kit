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

#include "DfxQComputeErrorIcon.h"
#include "DfxQComputeNode.h"

QSvgRenderer *DfxQComputeErrorIcon::renderer = nullptr;

DfxQComputeErrorIcon::DfxQComputeErrorIcon(float radius, QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	_radius = 25.5;

	if (renderer == nullptr) renderer = new QSvgRenderer(QLatin1String(":/icon/compute_error_icon"));
	icon = new QGraphicsSvgItem(this);

	_item = nullptr;
	_editor = nullptr;

	_computeNodePtr = nullptr;

	icon->setSharedRenderer(renderer);
	icon->setScale(0.045);
	icon->setPos(1.5, 1.4);
}

DfxQComputeErrorIcon::~DfxQComputeErrorIcon() {

}

int DfxQComputeErrorIcon::type() const {
	return Type;
}

void DfxQComputeErrorIcon::setItemPtr(DfxQListenerItem *ptr) {
	_item = ptr;
}

void DfxQComputeErrorIcon::draw() {
	QPainterPath p;

	p.addEllipse(0, 0, _radius, _radius);
	_circle = p;

	setPath(p);
}

void DfxQComputeErrorIcon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	draw();

	painter->setPen(QPen(Qt::white, 2));
	painter->setBrush(QColor(255, 255, 255));

	//painter->drawPath(_circle);
}

void DfxQComputeErrorIcon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {

	if (_computeNodePtr != nullptr) {
		bool err = false;

		if (!_computeNodePtr->dataObjectPath().empty()) {
			auto any = DfxSpace::getInstance()->findAbsolute(_computeNodePtr->dataObjectPath());
			auto dobj = any->rawData<fl::DataObject>();

			QMessageBox msgBox;
			msgBox.setWindowTitle("Error Message");
			msgBox.setText(dobj->errorString().c_str());
			msgBox.exec();
			
		}
	}
}

void DfxQComputeErrorIcon::setEditorPtr(DfxFluentNodeEditor *flt) {
	_editor = flt;
}

void DfxQComputeErrorIcon::setComputePtr(DfxQComputeNode *_compPtr) {
	_computeNodePtr = _compPtr;
}