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

#include <FlCommon.h>
#include <FlAny.h>
#include <FlDataObject.h>
#include <DfxSpace.h>
#include "DfxQMaterialBaseNode.h"

class DfxQMaterialNode : public DfxQMaterialBaseNode {
private:
	QPainterPath combined;
	QPainterPath body;
	QPainterPath header;
	QPainterPath footer;
protected:
	QGraphicsTextItem *_labelItem;
	std::string _label;
	int _labelPointSize;

	QColor _headerColor;

	

	bool mousePressed;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
public:

	DfxQMaterialNode(QGraphicsItem *parent = nullptr);
	~DfxQMaterialNode();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void buildNode(fl::Any *any);
	void clear();

	std::string label();
	void setLabel(std::string str);

	int labelPointSize();
	void setLabelPointSize(int);

	QFont labelFont();

	QColor headerColor();
	void setHeaderColor(QColor cl);

	void setSlotColor(DfxQMaterialNodeProperty *);

	void updateSlotConnectionPositions();
};