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
#include <QPainter>
#include <QPainterPath>
#include <libavoid/libavoid.h>
#include "DfxFlowHandler.h"
#include "DfxQNodeEditorCommon.h"
#include "DfxQFlowEnums.h"

class DfxQFlowLink : public QGraphicsPathItem {
private:
	static size_t ID_GEN;

	size_t _uid;

	DfxQFlowBaseNode *_node00;
	DfxQFlowBaseNode *_node01;

	uint _srcState;

	Avoid::Router *_router;
	Avoid::ConnRef *_connRef;
public:
	enum { Type = DfxQNodeEnums::FLOW_LINK };

	DfxQFlowLink(Avoid::Router *rt, QGraphicsItem *parent = 0);
	virtual ~DfxQFlowLink();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	DfxQFlowBaseNode *node00();
	void setNode00(DfxQFlowBaseNode *slt);

	DfxQFlowBaseNode *node01();
	void setNode01(DfxQFlowBaseNode *slt);

	size_t uid();

	void updatePosition(QPointF a, QPointF b);
	void updatePosition();

	int type() const;

	uint srcState();

	static DfxQFlowLink *findExistingLink(uint srcState, DfxQFlowBaseNode *srcNode);

	void bind();

	void unbind();

	void setSrcState(uint);
};