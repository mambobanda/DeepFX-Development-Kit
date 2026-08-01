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
#include "DfxQMaterialNodeEnums.h"
#include "DfxQMaterialNodeProperty.h"

class DfxQMaterialPropertyLink : public QGraphicsPathItem {
private:
	static size_t ID_GEN;

	size_t _uid;

	DfxQMaterialNodeSlot *_slot00;
	DfxQMaterialNodeSlot *_slot01;

public:
	enum { Type = DfxQMaterialNodeEnums::LINK};

	DfxQMaterialPropertyLink(QGraphicsItem *parent = 0);
	virtual ~DfxQMaterialPropertyLink();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	DfxQMaterialNodeSlot *slot00();
	void setSlot00(DfxQMaterialNodeSlot *slt);

	DfxQMaterialNodeSlot *slot01();
	void setSlot01(DfxQMaterialNodeSlot *slt);

	size_t uid();

	void updatePosition(QPointF a, QPointF b);
	void updatePosition();

	int type() const;
};