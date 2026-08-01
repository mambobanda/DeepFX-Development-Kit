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

#include "DfxLayerManager.h"
#include "DfxQCompositeNode.h"


DfxQCompositeNode::DfxQCompositeNode(DfxEntity *ent, QGraphicsItem *parent):DfxQBaseNode(ent, DfxQNodeType::COMPOSITE_NODE, parent) {

	//importIcon = new QGraphicsSvgItem(":/layers_tb/import_node_icon", this);
	//importIcon->setScale(0.05f);
	title = new QGraphicsTextItem(this);
	QFont font;
	font.setBold(true);
	title->setFont(font);
	setSlotSpacing(10);
	
	setPen(QPen(Qt::darkBlue));
	setBrush(Qt::blue);
	
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	
	entity = nullptr;
	propertiesForm = nullptr;
	
	entity = ent;
	
	qreal width = 50;
	qreal height = 100;
	
	if (entity != nullptr) {
	
		setTitle(entity->getLogicalName().c_str());
	
		QPainterPath p;
		p.addRoundedRect(-width / 2, 0, width, height, 5, 5);
		setPath(p);

		QFontMetrics fm(title->font());
		title->setPos(-width / 2, -(fm.height()*1.5f));
	}
}

DfxQCompositeNode::~DfxQCompositeNode() {

}

void DfxQCompositeNode::setTitle(QString txt) {
	title->setPlainText(txt);
}

void DfxQCompositeNode::setSlotSpacing(int sp) {
	slotSpacing = sp;
}

void DfxQCompositeNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option)
	Q_UNUSED(widget)
	
	painter->setRenderHints(QPainter::HighQualityAntialiasing | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	if (isSelected()) {
		painter->setPen(QPen(Qt::yellow));
		painter->setBrush(QColor(105, 172, 239));
	}
	else {
		painter->setPen(QPen(Qt::gray));
		painter->setBrush(QColor(105, 136, 239));
	}
	
	setTitle(getEntity()->getLogicalName().c_str());
	
	painter->drawPath(path());
}

bool DfxQCompositeNode::isReady() {
	return ready;
}

void DfxQCompositeNode::setReady(bool val) {
	ready = val;
}

void DfxQCompositeNode::rebuild() {

	std::vector<DfxQNodeSlot*> tempSlots;
	auto cnode = (DfxNodes::DfxBaseCompositeNode*)entity;

	cleanLooseSlots();

	for (auto nodeSlot : nodeSlots) {
		auto scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
		scene->removeItem(nodeSlot);

		tempSlots.push_back(nodeSlot);
	}

	nodeSlots.clear();

	for (auto nodeSlot : tempSlots) {
		addSlot(nodeSlot);
	}

	for (auto &entry : cnode->getAllLiftedSlots()) {

		std::pair<DfxEntity *, uint> slt = entry.second;
		auto dinstance = slt.first->getDataInstance();

		auto qnode = findQBaseNode(slt.first->getInstanceID());

		if (qnode != nullptr) {

			if (!hasSlot(slt.first->getInstanceID(), slt.second)) {
				QColor color = workOutSlotColor(*slt.first, slt.second);
				DfxEntityPropertyState st = dinstance->getPropertyInstance(slt.second)->getState();
				bool squareSlot = !slotIsArray(slt.first, slt.second);

				if (DfxEntityPropertyState::OUTPUT_STATE == dinstance->getPropertyInstance(slt.second)->getState()) {
					addOutputSlot(dinstance->getPropertyName(slt.second).c_str(), color, slt.second, qnode, squareSlot);
				}
				else if (DfxEntityPropertyState::INPUT_STATE == dinstance->getPropertyInstance(slt.second)->getState()) {
					addInputSlot(dinstance->getPropertyName(slt.second).c_str(), color, slt.second, qnode, squareSlot);
				}
			}
		}
	}
}

void DfxQCompositeNode::addSlot(QString label, DfxQNodeSlotType t, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot) {

	DfxQNodeSlot *slot = new DfxQNodeSlot(t, label, prop, sqSlot, this);
	slot->setParentNode(parent);
	slot->setSlotColor(c);

	addSlot(slot);
}

void DfxQCompositeNode::addSlot(DfxQNodeSlot *slot) {
	nodeSlots.push_back(slot);

	int width = 0;
	for (auto slt : nodeSlots) if (slt->getFontWidth() > width) width = slt->getFontWidth();

	int count = 0;
	int height = 0;

	for (auto slt : nodeSlots) {
		height = (count * slot->boundingRect().height()) + (count * slotSpacing);

		if (slt->getType() == DfxQNodeSlotType::INPUT_SLOT)
			slt->setPos(-width / 2 - (slot->getRadius()), height);
		else
			slt->setPos(width / 2 + (slot->getRadius()), height);

		count++;
	}

	int nheight = height + slot->boundingRect().height();

	QPainterPath p;
	int xx = slot->getRadius() + slot->getMargin() + 5;
	p.addRoundedRect(-width / 2, -xx, width, nheight + xx, 5, 5);
	setPath(p);

	QFontMetrics fm(title->font());
	title->setPos(-width / 2, -(fm.height()*2.5f));

	/*if (nodeType == DfxQNodeType::IMPORT_NODE) {
		importIcon->setPos((-width / 2) - 20, -(fm.height()*2.5f));
	}*/
}

void DfxQCompositeNode::addInputSlot(QString label, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot) {
	addSlot(label, DfxQNodeSlotType::INPUT_SLOT, c, prop, parent, sqSlot);
}

void DfxQCompositeNode::addOutputSlot(QString label, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot) {
	addSlot(label, DfxQNodeSlotType::OUTPUT_SLOT, c, prop, parent, sqSlot);
}

DfxQNodeSlot *DfxQCompositeNode::findSlot(size_t instance_id, size_t id) {

	for (auto slt : nodeSlots) {
		if (slt->getParentEntity()->getInstanceID() == instance_id) {
			if (slt->getPropertyID() == id) {
				return slt;
			}
		}
	}

	return nullptr;
}

bool DfxQCompositeNode::hasSlot(size_t instance_id, size_t id) {

	for (auto slt : nodeSlots) {
		if (slt->getParentEntity()->getInstanceID() == instance_id) {
			if (slt->getPropertyID() == id) {
				return true;
			}
		}
	}

	return false;
}

DfxQBaseNode *DfxQCompositeNode::findQBaseNode(size_t instance_id) {

	auto scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();

	for (auto item : scene->items()) {

		if (item && item->type() == DfxQBaseNode::Type) {
			auto qnode = (DfxQBaseNode*)item;
			if (qnode->getEntity()->getInstanceID() == instance_id) {
				return qnode;
			}
		}
	}

	return nullptr;
}

void DfxQCompositeNode::rebuildChildCompositeNodes() {

	auto scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();

	for (auto item : scene->items()) {

		if (item && item->type() == DfxQBaseNode::Type) {
			auto qnode = (DfxQBaseNode*)item;
			if (qnode->getNodeType() == DfxQNodeType::COMPOSITE_NODE) {
				((DfxQCompositeNode*)qnode)->rebuild();
			}
		}
	}
}

void DfxQCompositeNode::cleanLooseSlots() {

	auto it = nodeSlots.begin();

	while (it != nodeSlots.end()) {

		auto nodeSlot = *it;
		auto cnode = (DfxNodes::DfxBaseCompositeNode*)entity;

		bool removeSlot = true;
		for (auto &entry : cnode->getAllLiftedSlots()) {

			if (entry.first == nodeSlot->getParentEntity()->getInstanceID()) {
				if (entry.second.second == nodeSlot->getPropertyID()) {
					removeSlot = false;
					break;
				}
			}
		}

		if (removeSlot) {
			auto scene = DfxLayerManager::getInstance()->getSelectedLayer()->getScene();
			scene->removeItem(nodeSlot);

			nodeSlot->removeConnections();
			it = nodeSlots.erase(it);

			delete nodeSlot;
		}
		else {
			++it;
		}
	}
}