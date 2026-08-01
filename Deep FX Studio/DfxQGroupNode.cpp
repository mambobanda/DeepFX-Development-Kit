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


#include "DfxQGroupNode.h"

DfxQGroupNode::DfxQGroupNode(DfxEntity *ent, QGraphicsItem *parent): DfxQBaseNode(ent, DfxQNodeType::GROUP_NODE, parent) {
	title = new QGraphicsTextItem(this);
	title->setVisible(false);
	
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	
	entity = nullptr;
	propertiesForm = nullptr;
	
	entity = ent;
	
	float size = 70;
	float hsize = 0.5f * 70;
		
	QColor c(128, 0, 120);
		
	QFont font;
	font.setBold(true);
	font.setWeight(4);
	
	font.setPointSize(50);
	
	DfxQNodeSlot *in = new DfxQNodeSlot(DfxQNodeSlotType::GROUP_INPUT_SLOT, "", 1000, false, this);
	in->setParentNode(this);
	in->setSlotColor(c);
	in->setPos(-hsize - 15, 0);
	
	DfxQNodeSlot *out = new DfxQNodeSlot(DfxQNodeSlotType::GROUP_OUTPUT_SLOT, "", DfxNodes::DfxGroupNode::out, false, this);
	out->setParentNode(this);
	out->setSlotColor(c);
	out->setPos(hsize + 5, 0);
	
	nodeSlots.push_back(in);
	nodeSlots.push_back(out);
	
	auto letter = new QGraphicsTextItem("G", this);
	letter->setPos(-28, -47);
	letter->setFont(font);
	
	QPainterPath p;
	p.addEllipse(-hsize, -hsize, 70, 70);
	setPath(p);
}

DfxQGroupNode::~DfxQGroupNode() {

}

void DfxQGroupNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
		Q_UNUSED(widget)

		painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow));
		painter->setBrush(QColor(102, 102, 102));
	}
	else {
		painter->setPen(QPen(Qt::gray));
		painter->setBrush(QColor(77, 77, 77));
	}

	painter->drawPath(path());
}
