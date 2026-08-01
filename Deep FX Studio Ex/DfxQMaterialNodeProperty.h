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

#pragma once

#include <QGraphicsPathItem>
#include "DfxQMaterialNodeSlot.h"
#include "DfxQMaterialNodeSlotLabel.h"
#include "DfxQMaterialNodeEnums.h"

class DfxQMaterialNodeProperty : public QGraphicsPathItem {
public:

	enum class SlotAlignment {LEFT, RIGHT};
private:
	DfxQMaterialNodeSlot *_slot;
	DfxQMaterialNodeSlotLabel *_label;

	int _margin;
	int _textOffset;
	SlotAlignment _slotAlignment;

private:
	void setRadiusMargin(int margin);

public:

	DfxQMaterialNodeProperty(QGraphicsItem *parent = nullptr);
	DfxQMaterialNodeProperty(SlotAlignment _slotAlignment, QGraphicsItem *parent = nullptr);
	~DfxQMaterialNodeProperty();

	QRectF boundingRect() const override;

	DfxQMaterialNodeSlot *slot();
	DfxQMaterialNodeSlotLabel *label();

	int margin();
	void setMargin(int mrg);

	SlotAlignment slotAlignment();
	void setSlotAlignment(SlotAlignment alignment);

	int labelOffset();
	void setLabelOffset(int os);

	void updateSlotConnectionPositions();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};