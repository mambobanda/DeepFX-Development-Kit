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

#include "DfxQMaterialNodeProperty.h"

DfxQMaterialNodeProperty::DfxQMaterialNodeProperty(QGraphicsItem *parent):QGraphicsPathItem(parent) {
	_slot = new DfxQMaterialNodeSlot(this);
	_label = new DfxQMaterialNodeSlotLabel(this);
	_margin = 4;
	_textOffset = 4;
}


DfxQMaterialNodeProperty::DfxQMaterialNodeProperty(SlotAlignment alignment, QGraphicsItem *parent):QGraphicsPathItem(parent) {
	_slot = new DfxQMaterialNodeSlot(this);
	_label = new DfxQMaterialNodeSlotLabel(this);

	_slotAlignment = alignment;

	if (alignment == SlotAlignment::LEFT) {
		_slot->setMode(DfxQMaterialNodeSlot::SlotMode::INPUT);
	}
	else if (alignment == SlotAlignment::RIGHT) {
		_slot->setMode(DfxQMaterialNodeSlot::SlotMode::OUTPUT);
	}

	_margin = 4;
	_textOffset = 4;
}

DfxQMaterialNodeProperty::~DfxQMaterialNodeProperty() {

}

void DfxQMaterialNodeProperty::setRadiusMargin(int margin) {

	int radius_ = _slot->radius();
	int margin_ = margin;

	if (_slotAlignment == SlotAlignment::RIGHT)
		_label->setPos(-radius_ - margin_ - _label->getFontWidth(), _textOffset);
	else
		_label->setPos(radius_ + margin_, _textOffset);
}

DfxQMaterialNodeSlot *DfxQMaterialNodeProperty::slot() {
	return _slot;
}

int DfxQMaterialNodeProperty::labelOffset() {
	return _textOffset;
}

void DfxQMaterialNodeProperty::setLabelOffset(int os) {
	_textOffset = os;
}

DfxQMaterialNodeSlotLabel *DfxQMaterialNodeProperty::label() {
	return _label;
}

int DfxQMaterialNodeProperty::margin() {
	return _margin;
}

void DfxQMaterialNodeProperty::setMargin(int mrg) {
	_margin = mrg;
}

void DfxQMaterialNodeProperty::setSlotAlignment(SlotAlignment alignment) {
	_slotAlignment = alignment;
}

DfxQMaterialNodeProperty::SlotAlignment DfxQMaterialNodeProperty::slotAlignment() {
	return _slotAlignment;
}

QRectF DfxQMaterialNodeProperty::boundingRect() const {
	return _slot->boundingRect().united(_label->boundingRect());
}

void DfxQMaterialNodeProperty::updateSlotConnectionPositions() {
	_slot->updateConnectionPositions();
}

void DfxQMaterialNodeProperty::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	setRadiusMargin(_margin);
}