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

#include "DfxQMaterialNodeSlot.h"
#include "DfxQMaterialPropertyLink.h"


DfxQMaterialNodeSlot::DfxQMaterialNodeSlot(QGraphicsItem *parent):QGraphicsPathItem(parent){

	_fillColor = QColor(255, 0, 0);
	_strokeColor = QColor(60, 60, 60);

	_radius = 6;

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	setPath(drawSlot());

	_shape = DfxQMaterialNodeSlot::SlotShape::CIRCLE;
}

DfxQMaterialNodeSlot::~DfxQMaterialNodeSlot() {

}

std::string DfxQMaterialNodeSlot::propertyPath() {
	return _propertyPath;
}

void DfxQMaterialNodeSlot::setPropertyPath(std::string path) {
	_propertyPath = path;
}

QPainterPath DfxQMaterialNodeSlot::drawSlot() const{
	QPainterPath p;

	if (_shape == DfxQMaterialNodeSlot::SlotShape::SQUARE)
		p.addRect(-_radius, -_radius, _radius * 2, _radius * 2);
	else
		p.addEllipse(-_radius, -_radius, 2 * _radius, 2 * _radius);

	return p;
}

QRectF DfxQMaterialNodeSlot::boundingRect() const {
	return drawSlot().boundingRect();
}

void DfxQMaterialNodeSlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	painter->setPen(QPen(_strokeColor, 2));
	painter->setBrush(QColor(_fillColor));

	auto p = drawSlot();
	//setPath(p);
	painter->drawPath(p);
}

QColor DfxQMaterialNodeSlot::fillColor() {
	return _fillColor;
}

void DfxQMaterialNodeSlot::setFillColor(QColor fcolor) {
	_fillColor = fcolor;
}

QColor DfxQMaterialNodeSlot::strokeColor() {
	return _strokeColor;
}

void DfxQMaterialNodeSlot::setStrokeColor(QColor scolor) {
	_strokeColor = scolor;
}

int DfxQMaterialNodeSlot::radius() {
	return _radius;
}

void DfxQMaterialNodeSlot::setRadius(int rds) {
	_radius = rds;
}

DfxQMaterialNodeSlot::SlotShape DfxQMaterialNodeSlot::shape() {
	return _shape;
}

void DfxQMaterialNodeSlot::setShape(DfxQMaterialNodeSlot::SlotShape ss) {
	_shape = ss;
}

DfxQMaterialNodeSlot::SlotMode DfxQMaterialNodeSlot::mode() {
	return _mode;
}

void DfxQMaterialNodeSlot::setMode(DfxQMaterialNodeSlot::SlotMode mod) {
	_mode = mod;
}

bool DfxQMaterialNodeSlot::hasLinks() {
	return (!_links.empty());
}

std::vector<DfxQMaterialPropertyLink*> DfxQMaterialNodeSlot::links() {
	return _links;
}

void DfxQMaterialNodeSlot::addLink(DfxQMaterialPropertyLink *lks) {
	_links.push_back(lks);
}

void DfxQMaterialNodeSlot::removeLink(DfxQMaterialPropertyLink *lnk) {
	int counter = 0;
	for (auto l : _links) {

		if (l->uid() == lnk->uid()) {
			_links.erase(_links.begin() + counter);
			return;
		}

		counter++;
	}
}

void DfxQMaterialNodeSlot::removeAllLinks() {
	while (!_links.empty()) {
		_links.erase(_links.begin());
	}
}

void DfxQMaterialNodeSlot::updateConnectionPositions() {
	for (auto l : _links) l->updatePosition();
}

int DfxQMaterialNodeSlot::type() const {
	return Type;
}

