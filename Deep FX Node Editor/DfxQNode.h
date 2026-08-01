#pragma once

#include <QtWidgets\QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <vector>
#include <QFont>
#include "DfxQNodeCommon.h"
#include "DfxQNodeSlot.h"


class DfxQNode: public QGraphicsPathItem {
private:
	std::vector<DfxQNodeSlot*> nodeSlots;
	void addSlot(QString label, DfxQNodeSlotType t);
private:
	int slotSpacing;
	QGraphicsTextItem *title;
public:
	enum { Type = QGraphicsItem::UserType + 1 };

	DfxQNode(QGraphicsItem *parent = 0);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void addInputSlot(QString label);
	void addOutputSlot(QString label);

	void setSlotSpacing(int);
	void setTitle(QString);

	int type() const;

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
