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
#include <DfxGlobal.h>
#include "DfxQNodeEditorCommon.h"
#include "DfxQComputeNodeEnums.h"
#include "DfxQComputeNodeProperty.h"

class DfxQComputePropertyLink : public QGraphicsPathItem {
private:
	static size_t ID_GEN;

	size_t _uid;

	DfxQComputeNodeSlot *_slot00;
	DfxQComputeNodeSlot *_slot01;

	uint _linkType;
public:
	enum { Type = DfxQNodeEnums::COMPUTE_LINK , NORMAL=200, COMPOUND_INPUT_LINK=400, COMPOUND_OUTPUT_LINK=600 };

	DfxQComputePropertyLink(QGraphicsItem *parent = 0);
	virtual ~DfxQComputePropertyLink();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	DfxQComputeNodeSlot *slot00();
	void setSlot00(DfxQComputeNodeSlot *slt);

	DfxQComputeNodeSlot *slot01();
	void setSlot01(DfxQComputeNodeSlot *slt);

	size_t uid();

	void updatePosition(QPointF a, QPointF b);
	void updatePosition();

	uint linkType();
	void setLinkType(uint);

	int type() const;
};