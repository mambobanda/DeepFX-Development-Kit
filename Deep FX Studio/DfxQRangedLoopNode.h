/*
* The Mailo Today Solutions Software License
*
* Copyright 2019 Mailo Today Solutions.
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
#include "DfxRangedLoopNode.h"

class DfxQRangedLoopNode : public DfxQBaseNode {
private:
	DfxQNodeSlot *fromSlot;
	DfxQNodeSlot *toSlot;
private:
	int slotSpacing;
	QGraphicsTextItem *title;

public:

	DfxQRangedLoopNode(DfxEntity *entity, QRectF boundingRect, QGraphicsItem *parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void setTitle(QString);

	void addDelagate(DfxQBaseNode *);
	void clearDelagates();
protected:
	//QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
