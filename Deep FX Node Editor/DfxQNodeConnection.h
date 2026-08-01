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

class DfxQNodeConnection : public QGraphicsPathItem {
private:
	enum { Type = QGraphicsItem::UserType + 3 };
	
	size_t id;

	QPointF pos1;
	QPointF pos2;
	DfxQNodeSlot *slot_1;
	DfxQNodeSlot *slot_2;

public:
	DfxQNodeConnection(QGraphicsItem *parent = 0);
	virtual ~DfxQNodeConnection();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setPos1(const QPointF &p);
	void setPos2(const QPointF &p);

	void setPort1(DfxQNodeSlot *p);
	void setPort2(DfxQNodeSlot *p);

	void updatePosFromPorts();
	void updatePath();

	DfxQNodeSlot* slot1() const;
	DfxQNodeSlot* slot2() const;

	size_t getID();

	void setID(size_t);

	int type() const;
};