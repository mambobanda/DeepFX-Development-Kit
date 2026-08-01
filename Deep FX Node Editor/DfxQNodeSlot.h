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
#include "DfxQNodeCommon.h"
#include "DfxQNodeConnection.h"

enum class DfxQNodeSlotType { INPUT_SLOT=0x100, OUTPUT_SLOT };

class DfxQNodeSlot : public QGraphicsPathItem {
private:
	size_t connection_id_gen;
	int radius_;
	int margin_;

	DfxQNodeSlotType _type;
	QGraphicsTextItem *label;

	std::vector<DfxQNodeConnection*> m_connections;

	DfxQNode *parentNode;

	QColor slotColor;
public:
	enum { Type = QGraphicsItem::UserType + 2 };

	DfxQNodeSlot( DfxQNodeSlotType type, QString label,  QGraphicsItem *parent = 0);

	//DfxQNodeSlot(DfxQNodeSlotType type, QString label, QColor color, QGraphicsItem *parent = 0);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	int getFontWidth();
	int getFontHeight();
	int getRadius();
	int getMargin();
	DfxQNodeSlotType getType();
	void setRadiusMargin(int, int);

	//QVector<DfxQNodeConnection*>& connections();

	void setParentNode(DfxQNode*);
	DfxQNode* getParentNode() const;
	bool isConnected(DfxQNodeSlot*);
	bool isOutput();
	bool isInput();

	void addConnection(DfxQNodeConnection*);
	void removeConnection(size_t);

	int type() const;

	void setSlotColor(QColor c);

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};
