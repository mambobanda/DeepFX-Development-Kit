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

#include "DfxQMaterialNodeSlotLabel.h"

DfxQMaterialNodeSlotLabel::DfxQMaterialNodeSlotLabel(QGraphicsItem *parent):QGraphicsPathItem(parent) {
	_label = "Node Slot Label";
	_color = QColor(255, 255, 255);
	_pointSize = 7;
}

DfxQMaterialNodeSlotLabel::~DfxQMaterialNodeSlotLabel() {

}

QRectF DfxQMaterialNodeSlotLabel::boundingRect() const {
	QFontMetrics fm(labelFont());	
	return fm.boundingRect(_label.c_str());
}

int DfxQMaterialNodeSlotLabel::getFontHeight() {
	QFontMetrics fm(labelFont());
	return fm.height();
}

int DfxQMaterialNodeSlotLabel::getFontWidth() {
	QFontMetrics fm(labelFont());
	return fm.width(_label.c_str());
}

void DfxQMaterialNodeSlotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setFont(labelFont());
	painter->setPen(QPen(_color));

	painter->drawText(0, 0, _label.c_str());
}

std::string DfxQMaterialNodeSlotLabel::label() {
	return _label;
}

void DfxQMaterialNodeSlotLabel::setLabel(std::string lb) {
	_label = lb;
}

int	 DfxQMaterialNodeSlotLabel::pointSize() {
	return _pointSize;
}

void DfxQMaterialNodeSlotLabel::setPointSize(int sz) {
	_pointSize = sz;
}

QColor DfxQMaterialNodeSlotLabel::color() {
	return _color;
}

void DfxQMaterialNodeSlotLabel::setColor(QColor clr) {
	_color = clr;
}

QFont DfxQMaterialNodeSlotLabel::labelFont() const{
	QFont font;
	//font.setBold(true);
	font.setFamily("Verdana");
	font.setPointSize(_pointSize);
	font.setLetterSpacing(QFont::SpacingType::AbsoluteSpacing, 1.2f);

	return font;
}