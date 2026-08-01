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

#include <QtSvg/QGraphicsSvgItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <vector>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include "DfxQEnumControl.h"
#include "DfxQDecimal3SpinnerControl.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQDirectoryPathControl.h"
#include "DfxEntity.h"
#include "DfxQNodeCommon.h"
#include "DfxQNodeSlot.h"
#include "DfxQStringControl.h"
#include "DfxEntityBasicPropertyTypes.h"
#include "DfxQNodePropertiesPanel.h"
#include "DfxQBaseNode.h"

class DfxQStandardNode: public DfxQBaseNode {
private:

	bool standardNode;
private:
	int slotSpacing;
	QGraphicsTextItem *title;
	
	QPainterPath combined;
	QPainterPath header;
	QPainterPath body;
	QPainterPath footer;
public:

	DfxQStandardNode(DfxEntity *entity, QGraphicsItem *parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void addSlot(QString label, DfxQNodeSlotType t, QColor c, DfxPropertyID prop, bool sqSlot);

	void addInputSlot(QString label, QColor c, DfxPropertyID prop, bool sqSlot);
	void addOutputSlot(QString label, QColor c, DfxPropertyID prop, bool sqSlot);
	void addGroupInputSlot(QColor c, DfxPropertyID prop);
	void addGroupOutputSlot(QColor c, DfxPropertyID prop);

	void setSlotSpacing(int);
	void setTitle(QString);

	/*
	void createGroupNode(DfxEntity *entity);

	void createCompositeNode(DfxEntity *entity);
	*/
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
