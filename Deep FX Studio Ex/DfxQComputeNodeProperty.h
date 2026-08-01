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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QGraphicsPathItem>
#include "DfxQComputeNodeSlot.h"
#include "DfxQComputeNodeSlotLabel.h"
#include "DfxQComputeNodeEnums.h"

class DfxQComputeNodeProperty : public QGraphicsPathItem {
public:

	enum class SlotAlignment { LEFT, RIGHT };
	enum class SlotMode { NORMAL, APPEND_COMPOUND_INPUT, COMPOUND_INPUT_PROXY, 
		                  APPEND_COMPOUND_OUTPUT, COMPOUND_OUTPUT_PROXY };
private:
	DfxQComputeNodeSlot *_slot;
	DfxQComputeNodeSlotLabel *_label;

	int _margin;
	int _textOffset;
	SlotAlignment _slotAlignment;
	SlotMode	  _slotMode;

	DfxQComputeCompoundInNode *_inNode;
	DfxQComputeCompoundOutNode *_outNode;
private:
	void setRadiusMargin(int margin);

public:

	enum { Type = DfxQNodeEnums::COMPUTE_PROPERTY };

	int type() const override;

	DfxQComputeNodeProperty(QGraphicsItem *parent = nullptr);
	DfxQComputeNodeProperty(SlotAlignment _slotAlignment, QGraphicsItem *parent = nullptr);
	~DfxQComputeNodeProperty();

	QRectF boundingRect() const override;

	DfxQComputeNodeSlot *slot();
	DfxQComputeNodeSlotLabel *label();

	int margin();
	void setMargin(int mrg);

	SlotAlignment slotAlignment();
	void setSlotAlignment(SlotAlignment alignment);

	SlotMode slotMode();
	void setMode(SlotMode);

	DfxQComputeCompoundInNode *inNode();
	void setInNode(DfxQComputeCompoundInNode *in);

	DfxQComputeCompoundOutNode *outNode();
	void setOutNode(DfxQComputeCompoundOutNode *in);

	int labelOffset();
	void setLabelOffset(int os);

	void updateSlotConnectionPositions();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void forcePositionUpdate();
};