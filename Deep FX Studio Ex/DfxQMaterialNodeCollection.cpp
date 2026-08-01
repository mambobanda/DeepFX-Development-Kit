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

#include "DfxQMaterialNodeCollection.h"

DfxQMaterialNodeCollection::DfxQMaterialNodeCollection() {
	_materialPath = "";
}

DfxQMaterialNodeCollection::~DfxQMaterialNodeCollection() {
	for (auto n: nodes) {
		removeNode(n);
	}
}

void DfxQMaterialNodeCollection::drawBackground(QPainter *painter, const QRectF &rect) {

	int scale = 100000;
	int spacing = 10;

	painter->setPen(QPen(QColor(96, 96, 96, 50), 1.f));

	for (int x = -scale; x <= scale; x += spacing)
		painter->drawLine(x, -scale, x, scale);

	for (int y = -scale; y <= scale; y += spacing)
		painter->drawLine(-scale, y, scale, y);
}

std::string DfxQMaterialNodeCollection::materialPath() {
	return _materialPath;
}

void DfxQMaterialNodeCollection::setMaterialPath(std::string matPath) {
	_materialPath = matPath;
}

void DfxQMaterialNodeCollection::constructLinks() {
	auto space = DfxSpace::getInstance();
	std::unordered_map<std::string, DfxQMaterialNodeSlot*> slotMap;

	auto allItems = items();

	for (auto item : allItems) {
		if (item->type() == DfxQMaterialNodeEnums::BASE_NODE) {
			auto bnode = (DfxQMaterialBaseNode*)item;
			for (auto sl : bnode->properties()) {
				slotMap[sl->slot()->propertyPath()] = sl->slot();
			}
		}
	}

	for (auto p : slotMap) {
		auto slot = p.second;
		if (slot->mode() == DfxQMaterialNodeSlot::SlotMode::INPUT) {
			if (space->hasBinding(slot->propertyPath())) {
				for (auto link: space->findLinks(slot->propertyPath())) {

					auto itrSource = slotMap.find(link.output_path);

					if (itrSource != slotMap.end()) {
						auto linkItem = new DfxQMaterialPropertyLink;

						linkItem->setSlot00(itrSource->second);
						linkItem->setSlot01(slot);

						linkItem->updatePosition();

						addItem(linkItem);
					}
				}
			}
		}
	}
}

void DfxQMaterialNodeCollection::addNode(DfxQMaterialNode *node) {
	addItem(node);
	nodes.push_back(node);
}

void DfxQMaterialNodeCollection::deleteNode(DfxQMaterialNode *) {

}

void DfxQMaterialNodeCollection::removeNode(DfxQMaterialNode *node) {
	auto itr = nodes.begin();

	while (itr != nodes.end()) {
		if ((*itr)->nodeID() == node->nodeID()) {
			removeItem(node);
			nodes.erase(itr);
			break;
		}

		itr++;
	}
}

void DfxQMaterialNodeCollection::deleteAllLinks() {

	auto allItems = items();

	for (auto item : allItems) {
		if (item->type() == DfxQMaterialNodeEnums::LINK) {
			removeItem(item);
			delete item;
		}
	}
}