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

#include <QtWidgets\QGraphicsPathItem>
#include <QPainterPath>
#include <QPen>
#include <Qstring.h>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <vector>
#include <QVariant>
#include "DfxEntity.h"
#include "DfxQNodeCommon.h"
#include "DfxQNodeConnection.h"

enum class DfxQNodeSlotType { INPUT_SLOT=0x100, OUTPUT_SLOT, GROUP_INPUT_SLOT, GROUP_OUTPUT_SLOT };

class DfxQNodeSlot : public QGraphicsPathItem {
private:

	int radius_;
	int margin_;

	DfxQNodeSlotType _type;
	QGraphicsTextItem *label;

	std::vector<DfxQNodeConnection*> m_connections;

	DfxQBaseNode *parentNode;

	QColor slotColor;

	DfxPropertyID property;

	bool squareSlotShape;
	bool lifted;
public:
	enum { Type = QGraphicsItem::UserType + 2 };

	DfxQNodeSlot( DfxQNodeSlotType type, QString label, DfxPropertyID prop, bool squareSlot=true, QGraphicsItem *parent = 0);

	~DfxQNodeSlot();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	int getFontWidth();
	int getFontHeight();
	int getRadius();
	int getMargin();
	DfxQNodeSlotType getType();
	void setRadiusMargin(int, int);

	//QVector<DfxQNodeConnection*>& connections();

	void setParentNode(DfxQBaseNode*);
	DfxQBaseNode* getParentNode() const;
	DfxEntity* getParentEntity();
	DfxPropertyID getPropertyID();
	size_t getPropertyType();
	bool isConnected(DfxQNodeSlot*);
	bool isOutput();
	bool isInput();

	void addConnection(DfxQNodeConnection*);
	DfxQNodeConnection* getConnection(size_t);
	void removeConnection(size_t);
	bool hasConnections();
	void removeConnections();

	int type() const;

	void setSlotColor(QColor c);

	DfxQNodeSlotType getSlotType();

	std::vector<DfxQNodeConnection*> &getAllConnections();

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	bool isLifted();

	void setLifted(bool val);

};
