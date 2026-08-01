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

#include "DfxQComputeNodeProperty.h"

DfxQComputeNodeProperty::DfxQComputeNodeProperty(QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	_slot = new DfxQComputeNodeSlot(this);
	_label = new DfxQComputeNodeSlotLabel(this);
	_margin = 4;
	_textOffset = 4;
	_slotMode = SlotMode::NORMAL;

	_inNode = nullptr;
	_outNode = nullptr;
}


DfxQComputeNodeProperty::DfxQComputeNodeProperty(SlotAlignment alignment, QGraphicsItem *parent) :QGraphicsPathItem(parent) {
	_slot = new DfxQComputeNodeSlot(this);
	_label = new DfxQComputeNodeSlotLabel(this);

	_slotAlignment = alignment;
	_slotMode = SlotMode::NORMAL;

	if (alignment == SlotAlignment::LEFT) {
		_slot->setMode(DfxQComputeNodeSlot::SlotMode::INPUT);
	}
	else if (alignment == SlotAlignment::RIGHT) {
		_slot->setMode(DfxQComputeNodeSlot::SlotMode::OUTPUT);
	}

	_margin = 4;
	_textOffset = 4;
}

DfxQComputeNodeProperty::~DfxQComputeNodeProperty() {

}

int DfxQComputeNodeProperty::type() const {
	return Type;
}

void DfxQComputeNodeProperty::setRadiusMargin(int margin) {

	int radius_ = _slot->radius();
	int margin_ = margin;

	if (_slotAlignment == SlotAlignment::RIGHT)
		_label->setPos(-radius_ - margin_ - _label->getFontWidth(), _textOffset);
	else
		_label->setPos(radius_ + margin_, _textOffset);
}

DfxQComputeNodeSlot *DfxQComputeNodeProperty::slot() {
	return _slot;
}

int DfxQComputeNodeProperty::labelOffset() {
	return _textOffset;
}

void DfxQComputeNodeProperty::setLabelOffset(int os) {
	_textOffset = os;
}

DfxQComputeNodeSlotLabel *DfxQComputeNodeProperty::label() {
	return _label;
}

int DfxQComputeNodeProperty::margin() {
	return _margin;
}

void DfxQComputeNodeProperty::setMargin(int mrg) {
	_margin = mrg;
}

void DfxQComputeNodeProperty::setSlotAlignment(SlotAlignment alignment) {
	_slotAlignment = alignment;
}

DfxQComputeNodeProperty::SlotAlignment DfxQComputeNodeProperty::slotAlignment() {
	return _slotAlignment;
}

QRectF DfxQComputeNodeProperty::boundingRect() const {
	return _slot->boundingRect().united(_label->boundingRect());
}

void DfxQComputeNodeProperty::updateSlotConnectionPositions() {
	_slot->updateConnectionPositions();
}

DfxQComputeNodeProperty::SlotMode DfxQComputeNodeProperty::slotMode() {
	return _slotMode;
}

void DfxQComputeNodeProperty::setMode(SlotMode sm) {
	_slotMode = sm;
}

void DfxQComputeNodeProperty::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	setRadiusMargin(_margin);
}

DfxQComputeCompoundInNode *DfxQComputeNodeProperty::inNode() {
	return _inNode;
}

void DfxQComputeNodeProperty::setInNode(DfxQComputeCompoundInNode *in) {
	_inNode = in;
}

DfxQComputeCompoundOutNode *DfxQComputeNodeProperty::outNode() {
	return _outNode;
}

void DfxQComputeNodeProperty::setOutNode(DfxQComputeCompoundOutNode *out) {
	_outNode = out;
}