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

#include "DfxQComputeNodeSlotLabel.h"

DfxQComputeNodeSlotLabel::DfxQComputeNodeSlotLabel(QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	_label = "Node Slot Label";
	_color = QColor(255, 255, 255);
	_pointSize = 7;
}

DfxQComputeNodeSlotLabel::~DfxQComputeNodeSlotLabel() {

}

QRectF DfxQComputeNodeSlotLabel::boundingRect() const {
	QFontMetrics fm(labelFont());
	return fm.boundingRect(_label.c_str());
}

int DfxQComputeNodeSlotLabel::getFontHeight() {
	QFontMetrics fm(labelFont());
	return fm.height();
}

int DfxQComputeNodeSlotLabel::getFontWidth() {
	QFontMetrics fm(labelFont());
	return fm.width(_label.c_str());
}

void DfxQComputeNodeSlotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setFont(labelFont());
	painter->setPen(QPen(_color));

	painter->drawText(0, 0, _label.c_str());
}

std::string DfxQComputeNodeSlotLabel::label() {
	return _label;
}

void DfxQComputeNodeSlotLabel::setLabel(std::string lb) {
	_label = lb;
}

int	 DfxQComputeNodeSlotLabel::pointSize() {
	return _pointSize;
}

void DfxQComputeNodeSlotLabel::setPointSize(int sz) {
	_pointSize = sz;
}

QColor DfxQComputeNodeSlotLabel::color() {
	return _color;
}

void DfxQComputeNodeSlotLabel::setColor(QColor clr) {
	_color = clr;
}

QFont DfxQComputeNodeSlotLabel::labelFont() const {
	QFont font;
	//font.setBold(true);
	font.setFamily("Verdana");
	font.setPointSize(_pointSize);
	font.setLetterSpacing(QFont::SpacingType::AbsoluteSpacing, 1.2f);

	return font;
}