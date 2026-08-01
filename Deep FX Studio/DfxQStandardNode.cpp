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

#include <QtGui>
#include <iostream>
#include "DfxQStandardNode.h"
#include "DfxEnvelopeMap.h"


DfxQStandardNode::DfxQStandardNode(DfxEntity *ent, QGraphicsItem *parent):DfxQBaseNode(ent, DfxQNodeType::STANDARD_NODE, parent) {

	standardNode = true;
	title = new QGraphicsTextItem(this);
	QFont font;
	font.setBold(true);
	title->setFont(font);
	setSlotSpacing(10);

	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);

	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	entity = nullptr;
	propertiesForm = nullptr;

	entity = ent;

	if (entity != nullptr) {

		setTitle(entity->getLogicalName().c_str());

		DfxEntityDataInstance *dinstance = entity->getDataInstance();

		std::vector<size_t> lst = dinstance->getPropertyList();
		std::sort(lst.begin(), lst.end());

		for (auto id : lst) {

			QColor color = workOutSlotColor(*entity, id);
			DfxEntityPropertyState st = dinstance->getPropertyInstance(id)->getState();
			bool squareSlot = !slotIsArray(entity, id);

			if (DfxEntityPropertyState::OUTPUT_STATE == dinstance->getPropertyInstance(id)->getState()) {
				addOutputSlot(dinstance->getPropertyName(id).c_str(), color, id, squareSlot);
			}
			else if (DfxEntityPropertyState::INPUT_STATE == dinstance->getPropertyInstance(id)->getState()) {
				addInputSlot(dinstance->getPropertyName(id).c_str(), color, id, squareSlot);
			}
		}

	}

	/*if (nodeType == DfxQNodeType::STANDARD_NODE) {
		createStandardNode(entity);
	}
	else if (nodeType == DfxQNodeType::IMPORT_NODE) {
		createImportNode(entity);
	}
	else if (nodeType == DfxQNodeType::GROUP_NODE) {
		createGroupNode(entity);
	}
	else if (nodeType == DfxQNodeType::COMPOSITE_NODE) {
		createCompositeNode(entity);
	}*/
	
}

//
//void DfxQStandardNode::createImportNode(DfxEntity *ent) {
//
//	importIcon = new QGraphicsSvgItem(":/layers_tb/import_node_icon", this);
//	importIcon->setScale(0.05f);
//	title = new QGraphicsTextItem(this);
//	QFont font;
//	font.setBold(true);
//	title->setFont(font);
//	setSlotSpacing(10);
//
//	setPen(QPen(Qt::darkGreen));
//	setBrush(Qt::green);
//
//	setFlag(QGraphicsItem::ItemIsMovable);
//	setFlag(QGraphicsItem::ItemIsSelectable);
//	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
//
//	entity = nullptr;
//	propertiesForm = nullptr;
//
//	entity = ent;
//
//	if (entity != nullptr) {
//
//		setTitle(entity->getLogicalName().c_str());
//
//		DfxEntityDataInstance *dinstance = entity->getDataInstance();
//
//		std::vector<size_t> lst = dinstance->getPropertyList();
//		std::sort(lst.begin(), lst.end());
//
//		for (auto id : lst) {
//
//			QColor color = workOutSlotColor(*entity, id);
//			DfxEntityPropertyState st = dinstance->getPropertyInstance(id)->getState();
//			bool squareSlot = !slotIsArray(entity, id);
//
//			if (DfxEntityPropertyState::OUTPUT_STATE == dinstance->getPropertyInstance(id)->getState()) {
//				addOutputSlot(dinstance->getPropertyName(id).c_str(), color, id, squareSlot);
//			}
//			
//		}
//
//	}
//}


void DfxQStandardNode::addSlot(QString label, DfxQNodeSlotType t, QColor c, DfxPropertyID prop, bool sqSlot) {

	DfxQNodeSlot *slot = new DfxQNodeSlot(t, label, prop, sqSlot, this);	
	slot->setParentNode(this);
	slot->setSlotColor(c);

	nodeSlots.push_back(slot);

	int width   = 0;
	for (auto slt : nodeSlots) if (slt->getFontWidth() > width) width = slt->getFontWidth();
	
	int count  = 0;
	int height = 0;

	for (auto slt : nodeSlots) {
		height = (count * slot->boundingRect().height()) + (count * slotSpacing);

		if(slt->getType() == DfxQNodeSlotType::INPUT_SLOT)
			slt->setPos(-width/2 - (slot->getRadius()), height);
		else
			slt->setPos(width/2 + (slot->getRadius()), height);

		count++;
	}

	int nheight = height + slot->boundingRect().height();

	QPainterPath _combined;
	QPainterPath _body;
	QPainterPath _header;
	QPainterPath _footer;

	int xx = slot->getRadius() + slot->getMargin() + 5;
	_body.addRoundedRect(-width/2, -xx, width, nheight + xx, 0, 0);
	_combined.addRoundedRect(-width / 2, -xx, width, nheight + xx, 0, 0);
	_header.addRoundedRect(-width/2, -xx - 10, width, 10, 0, 0);
	_combined.addRoundedRect(-width / 2, -xx - 10, width, 10, 0, 0);
	_footer.addRoundedRect(-width / 2, nheight, width, 10, 0, 0);
	_combined.addRoundedRect(-width / 2, nheight + xx + 10, width, 10, 0, 0);

	combined = _combined;
	body	= _body;
	header  = _header;
	footer	= _footer;

	setPath(combined);

	prefferedHeight = nheight;
	prefferedWidth = width;

	QFontMetrics fm(title->font());
	title->setPos(-width / 2, -(fm.height()*2.5f) - 10);
	
	/*if (nodeType == DfxQNodeType::IMPORT_NODE) {
		importIcon->setPos((-width / 2) - 20, -(fm.height()*2.5f));
	}*/
}


void DfxQStandardNode::addInputSlot(QString label, QColor c,  DfxPropertyID prop, bool sqSlot) {
	addSlot(label, DfxQNodeSlotType::INPUT_SLOT, c, prop, sqSlot);
}

void DfxQStandardNode::addOutputSlot(QString label, QColor c, DfxPropertyID prop, bool sqSlot) {
	addSlot(label, DfxQNodeSlotType::OUTPUT_SLOT, c, prop, sqSlot);
}

void DfxQStandardNode::addGroupInputSlot(QColor c, DfxPropertyID prop) {
}

void DfxQStandardNode::addGroupOutputSlot(QColor c, DfxPropertyID prop) {
}

void DfxQStandardNode::setSlotSpacing(int sp) {
	slotSpacing = sp;
}

void DfxQStandardNode::setTitle(QString txt) {
	title->setPlainText(txt);
}

void DfxQStandardNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow));
		painter->setBrush(QColor(225, 225, 225));
	}
	else {
		painter->setPen(QPen(Qt::gray));
		painter->setBrush(QColor(255,255,255));
	}

	painter->drawPath(body);

	if (standardNode) {
		setTitle(getEntity()->getLogicalName().c_str());
	}

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow));
		painter->setBrush(QColor(52, 51, 52));
	}
	else {
		painter->setPen(QPen(Qt::gray));
		painter->setBrush(QColor(52, 51, 52));
	}

	painter->drawPath(header);
	painter->drawPath(footer);
}


QVariant DfxQStandardNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	Q_UNUSED(change);

	/*std::cout << (int)change <<std::endl;
	for (DfxQNodeSlot *slt : nodeSlots)
		slt->itemChange(change, value);*/

	return value;
}