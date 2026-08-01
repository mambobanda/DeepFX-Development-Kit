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

#include "DfxQRangedLoopNode.h"


DfxQRangedLoopNode::DfxQRangedLoopNode(DfxEntity *ent, QRectF boundingRect, QGraphicsItem *parent): DfxQBaseNode(ent, DfxQNodeType::RANGED_LOOP_NODE, parent) {

	//standardNode = true;
	title = new QGraphicsTextItem(this);
	QFont font;
	font.setBold(true);
	title->setFont(font);
	//setSlotSpacing(10);

	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);

	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	entity = nullptr;
	propertiesForm = nullptr;

	entity = ent;

	if (entity != nullptr) {
	
		float padding = 50.f;
		float slotPadding = 50.f;
		float pwidth = boundingRect.width() + padding;
		float pheight = boundingRect.height() + padding + slotPadding;
		float halfWidth = (pwidth * .5f);
		float halfHeight = (pheight * .5f);
		float x = -halfWidth;
		float y = -halfHeight;
		QPainterPath p;
		//p.addEllipse(0, 0, 10, 10);
		p.addRoundedRect(x, y, pwidth, pheight, 10, 10);
		setPos(boundingRect.x() + (halfWidth - (padding * .5f)), boundingRect.y() + (halfHeight - (padding * .5f) - slotPadding));

		setZValue(-1.0);	

		float textPosX = -halfWidth;
		float textPosY = -halfHeight - (padding * .5f);
		title->setPlainText(entity->getLogicalName().c_str());
		title->setPos(textPosX, textPosY);

		fromSlot = new DfxQNodeSlot(DfxQNodeSlotType::INPUT_SLOT, "range_begin", DfxNodes::DfxRangedLoopNode::inFrom, true, this);
		toSlot = new DfxQNodeSlot(DfxQNodeSlotType::INPUT_SLOT, "range_end", DfxNodes::DfxRangedLoopNode::inTo, true, this);

		QColor slotColor = workOutSlotColor(*entity, DfxNodes::DfxRangedLoopNode::inFrom);
		fromSlot->setSlotColor(slotColor);
		toSlot->setSlotColor(slotColor);

		float slotHeight = (fromSlot->boundingRect().height()) + (10);
		float slotRadius = fromSlot->getRadius();

		fromSlot->setPos(-halfWidth - (slotRadius), -halfHeight + 20);
		toSlot->setPos(-halfWidth - (slotRadius), -halfHeight + 20 + slotHeight);

		p.addRect(x, -halfHeight + (slotHeight * 2) + 20, boundingRect.width() + padding, 1);

		setPath(p);
	}

}

void DfxQRangedLoopNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

	painter->setPen(QPen(QColor(212, 85, 0)));
	painter->setBrush(QBrush(QColor(212, 85, 0, 50)));

	painter->drawPath(path());
}

void DfxQRangedLoopNode::setTitle(QString txt) {
	title->setPlainText(txt);
}

void DfxQRangedLoopNode::addDelagate(DfxQBaseNode *delg) {
	delg->setParentItem(this);
	delg->setFlag(QGraphicsItem::ItemIsMovable, false);

	auto relativePos = mapFromScene(delg->pos());

	delg->setPos(relativePos);
}

void DfxQRangedLoopNode::clearDelagates() {

	for (auto ch : childItems()) {

		if (ch->type() == DfxQBaseNode::Type) {
			ch->setParentItem(nullptr);
			ch->setFlag(QGraphicsItem::ItemIsMovable);
		}
	}
}