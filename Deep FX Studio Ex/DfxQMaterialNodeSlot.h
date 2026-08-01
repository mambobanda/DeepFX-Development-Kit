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
#include <QPainter>
#include "DfxQNodeEditorCommon.h"
#include "DfxQMaterialNodeEnums.h"


class DfxQMaterialNodeSlot : public QGraphicsPathItem {
public:
	enum class SlotShape { SQUARE, CIRCLE };
	enum class SlotMode  { INPUT, OUTPUT};
private:

	std::vector<DfxQMaterialPropertyLink*> _links;

	std::string _propertyPath;

	QColor _fillColor;
	QColor _strokeColor;

	int _radius;

	SlotShape _shape;
	SlotMode  _mode;
public:

	enum { Type = DfxQMaterialNodeEnums::PROPERTY_SLOT};

	DfxQMaterialNodeSlot(QGraphicsItem *parent = nullptr);
	~DfxQMaterialNodeSlot();

	std::string propertyPath();
	void setPropertyPath(std::string);

	QPainterPath drawSlot() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	QRectF boundingRect() const override;

	QColor fillColor();
	void setFillColor(QColor);

	QColor strokeColor();
	void setStrokeColor(QColor);

	int radius();
	void setRadius(int);

	SlotShape shape();
	void setShape(SlotShape ss);

	SlotMode mode();
	void setMode(SlotMode mod);

	void updateConnectionPositions();

	bool hasLinks();
	std::vector<DfxQMaterialPropertyLink*> links();
	void addLink(DfxQMaterialPropertyLink*);
	void removeLink(DfxQMaterialPropertyLink *);
	void removeAllLinks();

	int type() const;
};