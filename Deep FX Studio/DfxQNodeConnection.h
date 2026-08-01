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
#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include "DfxQNodeCommon.h"
#include "DfxQNodeSlot.h"

enum class DfxQNodeConnectionType {NORMAL, GROUP};

class DfxQNodeConnection : public QGraphicsPathItem {
private:
	static size_t connection_id_gen;

	size_t id;

	QPointF pos1;
	QPointF pos2;
	DfxQNodeSlot *slot_1;
	DfxQNodeSlot *slot_2;

	DfxQNodeConnectionType _type;
public:
	enum { Type = QGraphicsItem::UserType + 3 };

	DfxQNodeConnection(QGraphicsItem *parent = 0, DfxQNodeConnectionType _type = DfxQNodeConnectionType::NORMAL );
	virtual ~DfxQNodeConnection();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setPos1(const QPointF &p);
	void setPos2(const QPointF &p);

	void setPort1(DfxQNodeSlot *p);
	void setPort2(DfxQNodeSlot *p);

	DfxQNodeSlot *setPort1();
	DfxQNodeSlot *setPort2();

	void updatePosFromPorts();
	void updatePath();

	DfxQNodeSlot* slot1() const;
	DfxQNodeSlot* slot2() const;

	void bind();
	void unbind();

	size_t getID();

	void setID(size_t);

	int type() const;

	DfxQNodeConnectionType getConnectionType();
	void setConnectionType(DfxQNodeConnectionType);
};