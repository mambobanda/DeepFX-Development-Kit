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

#include "DfxQComputeNodeSlot.h"
#include "DfxQComputePropertyLink.h"


DfxQComputeNodeSlot::DfxQComputeNodeSlot(QGraphicsItem *parent) :QGraphicsPathItem(parent) {

	_fillColor = QColor(255, 0, 0);
	_strokeColor = QColor(60, 60, 60);

	_radius = 6;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	setPath(drawSlot());

	_shape = DfxQComputeNodeSlot::SlotShape::CIRCLE;
}

DfxQComputeNodeSlot::~DfxQComputeNodeSlot() {

}

std::string DfxQComputeNodeSlot::propertyPath() {
	return _propertyPath;
}

void DfxQComputeNodeSlot::setPropertyPath(std::string path) {
	_propertyPath = path;
}

QPainterPath DfxQComputeNodeSlot::drawSlot() const {
	QPainterPath p;

	if (_shape == DfxQComputeNodeSlot::SlotShape::SQUARE)
		p.addRect(-_radius, -_radius, _radius * 2, _radius * 2);
	else if (_shape == DfxQComputeNodeSlot::SlotShape::CIRCLE)
		p.addEllipse(-_radius, -_radius, 2 * _radius, 2 * _radius);
	else {
		p.addEllipse(-_radius, -_radius, 2 * _radius, 2 * _radius);
	}

	return p;
}

QPainterPath DfxQComputeNodeSlot::drawPlusSymbol() const {
	QPainterPath p;
	p.addText(-(_radius * 0.7f), (_radius * 0.6f), QFont(), "+");
	return p;
}

QRectF DfxQComputeNodeSlot::boundingRect() const {
	return drawSlot().boundingRect();
}

void DfxQComputeNodeSlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
		Q_UNUSED(widget)

		painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	painter->setPen(QPen(_strokeColor, 2));
	painter->setBrush(QColor(_fillColor));

	auto p = drawSlot();
	auto p_plus = drawPlusSymbol();

	//setPath(p);
	painter->drawPath(p);

	if (_shape == DfxQComputeNodeSlot::SlotShape::CIRCLE_PLUS) {
		painter->setPen(QPen(QColor::fromRgb(0, 0, 0), 1));
		painter->drawPath(p_plus);
	}
}

QColor DfxQComputeNodeSlot::fillColor() {
	return _fillColor;
}

void DfxQComputeNodeSlot::setFillColor(QColor fcolor) {
	_fillColor = fcolor;
}

QColor DfxQComputeNodeSlot::strokeColor() {
	return _strokeColor;
}

void DfxQComputeNodeSlot::setStrokeColor(QColor scolor) {
	_strokeColor = scolor;
}

int DfxQComputeNodeSlot::radius() {
	return _radius;
}

void DfxQComputeNodeSlot::setRadius(int rds) {
	_radius = rds;
}

DfxQComputeNodeSlot::SlotShape DfxQComputeNodeSlot::shape() {
	return _shape;
}

void DfxQComputeNodeSlot::setShape(DfxQComputeNodeSlot::SlotShape ss) {
	_shape = ss;
}

DfxQComputeNodeSlot::SlotMode DfxQComputeNodeSlot::mode() {
	return _mode;
}

void DfxQComputeNodeSlot::setMode(DfxQComputeNodeSlot::SlotMode mod) {
	_mode = mod;
}

std::vector<DfxQComputePropertyLink*> DfxQComputeNodeSlot::links() {
	return _links;
}

void DfxQComputeNodeSlot::addLink(DfxQComputePropertyLink *lks) {
	_links.push_back(lks);
}

void DfxQComputeNodeSlot::removeLink(DfxQComputePropertyLink *lnk) {
	int counter = 0;
	for (auto l : _links) {

		if (l->uid() == lnk->uid()) {
			_links.erase(_links.begin() + counter);
			return;
		}

		counter++;
	}
}

void DfxQComputeNodeSlot::removeAllLinks() {
	while (!_links.empty()) {
		_links.erase(_links.begin());
	}
}

bool DfxQComputeNodeSlot::hasLinks() {
	return (!_links.empty());
}

void DfxQComputeNodeSlot::updateConnectionPositions() {
	for (auto l : _links) l->updatePosition();
}

int DfxQComputeNodeSlot::type() const {
	return Type;
}