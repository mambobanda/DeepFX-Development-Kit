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
#include <QFont>
#include <QPainter>
#include "DfxQMaterialNodeEnums.h"

class DfxQMaterialNodeSlotLabel : public QGraphicsPathItem {
private:
	std::string _label;
	int			_pointSize;
	QColor		_color;

public:

	enum { Type = DfxQMaterialNodeEnums::PROPERTY_SLOT_LABEL };

	DfxQMaterialNodeSlotLabel(QGraphicsItem *parent = nullptr);
	~DfxQMaterialNodeSlotLabel();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QRectF boundingRect() const;

	int getFontHeight();
	int getFontWidth();

	std::string label();
	void setLabel(std::string lb);

	int	 pointSize();
	void setPointSize(int sz);

	QColor color();
	void setColor(QColor clr);

	QFont labelFont() const;
};
