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

#include "DfxLayer.h"
#include <DfxScene.h>
#include <DfxGlobal.h>
#include <DfxViewItem.h>
#include <DeepStudioGlobal.h>


size_t DfxLayerItem::unique_id_gen = 0;

DfxLayerItem::DfxLayerItem() {
	id = ++unique_id_gen;

	scene = nullptr;

	node  = nullptr;

	parent = nullptr; 

	top = false;
	linked = true;;
	visible = true;
	locked = false;

	itemType = DfxLayerItemType::LAYER_ITEM;
}

DfxLayerItem::~DfxLayerItem() {
	delete scene;
	delete node;
}

size_t DfxLayerItem::getID() {
	return id;
}

size_t DfxLayerItem::getOldInstanceID() {
	return oldInstanceID;
}

void DfxLayerItem::setOldInstanceID(size_t oid) {
	oldInstanceID = oid;
}

std::string DfxLayerItem::getName() {

	if (itemType == DfxLayerItemType::LAYER_ITEM)
		return name;
	else if (itemType == DfxLayerItemType::NODE_ITEM)
		return node->getEntity()->getLogicalName();
	else
		return "NULL";
}

void DfxLayerItem::setName(std::string str) {

	if (itemType == DfxLayerItemType::LAYER_ITEM)
		name = str;
	else if (itemType == DfxLayerItemType::NODE_ITEM)
		node->getEntity()->setLogicalName(str);
	else
		name = "NULL";
}

DfxQBaseNode *DfxLayerItem::getNode() {
	return node;
}

DfxLayerItemType DfxLayerItem::getItemType() {
	return itemType;
}

void DfxLayerItem::setItemType(DfxLayerItemType typ) {
	itemType = typ;
}


DfxLayerNodeType DfxLayerItem::getNodeType() {
	return nodeType;
}

void DfxLayerItem::setNodeType(DfxLayerNodeType typ) {
	nodeType = typ;
}

bool DfxLayerItem::isLinked() {
	return linked;
}

bool DfxLayerItem::isVisible() {
	return visible;
}

bool DfxLayerItem::isLocked() {
	return locked;
}

void DfxLayerItem::linkUnlink() {
	
	if (linked)
		linked = false;
	else
		linked = true;
}

void DfxLayerItem::hideUnhide() {

	if (visible) {
		visible = false;

		if (nodeType == DfxLayerNodeType::VIEW_NODE) {
			if (node->getEntity() != nullptr) {
				((DfxViewItem*)node->getEntity())->hide(true);

				DfxScene::getInstance()->clearSelectionIfSelected(node->getEntity()->getInstanceID());
				((DfxViewItem*)node->getEntity())->setSelected(false);
				DfxGlobal::command::exec("updateViewport");
			}
		}
	}
	else {
		visible = true;

		if (nodeType == DfxLayerNodeType::VIEW_NODE) {
			if (node->getEntity() != nullptr) {
				((DfxViewItem*)node->getEntity())->hide(false);
			}

			DfxGlobal::command::exec("updateViewport");
		}
	}
}

void DfxLayerItem::hide() {
	if (visible) {
		visible = false;

		if (nodeType == DfxLayerNodeType::VIEW_NODE) {
			if (node->getEntity() != nullptr) {
				((DfxViewItem*)node->getEntity())->hide(true);

				DfxScene::getInstance()->clearSelectionIfSelected(node->getEntity()->getInstanceID());
				((DfxViewItem*)node->getEntity())->setSelected(false);
				DfxGlobal::command::exec("updateViewport");
			}
		}
	}
}

void DfxLayerItem::unHide() {
	if(!visible) {
	visible = true;

	if (nodeType == DfxLayerNodeType::VIEW_NODE) {
		if (node->getEntity() != nullptr) {
			((DfxViewItem*)node->getEntity())->hide(false);
		}

		DfxGlobal::command::exec("updateViewport");
	}
	}
}

void DfxLayerItem::lockUnlock() {

	if (locked)
		locked = false;
	else
		locked = true;
}

void DfxLayerItem::setLinked(bool val) {
	linked = val;
}

void DfxLayerItem::setHidden(bool val) {
	visible = val;
}

void DfxLayerItem::setLocked(bool val) {
	locked = val;
}

QGraphicsScene *DfxLayerItem::getScene() {
	return scene;
}

size_t  DfxLayerItem::childCount() {
	return items.size();
}

DfxLayerItem *DfxLayerItem::getParent() {
	return parent;
}

void DfxLayerItem::setParent(DfxLayerItem *prt) {
	parent = prt;
}

DfxLayerItem *DfxLayerItem::getChild(size_t inx) {

	if (inx >= 0 && inx < items.size()) {
		return items[inx];
	}

	return nullptr;
}

void DfxLayerItem::addChild(DfxLayerItem* chd) {
	chd->setParent(this);
	items.push_back(chd);
}

void DfxLayerItem::removeChild(DfxLayerItem* chd) {
	delete items.takeAt(chd->row());
}

size_t DfxLayerItem::childRow(DfxLayerItem *it) {

	for (size_t inx = 0; inx < items.size(); inx++) {

		if (it->getID() == items[inx]->getID()) {
			return inx;
		}
	}

	return -1;
}

size_t DfxLayerItem::row() {

	if (parent != nullptr) {
		return parent->childRow(this);
	}

	return 0;
}

void DfxLayerItem::setNode(DfxQBaseNode *nd) {
	node = nd;
}

QGraphicsScene *DfxLayerItem::getScene(size_t) {
	return scene;
}

void DfxLayerItem::setScene(QGraphicsScene *scn) {
	scene = scn;
}

void DfxLayerItem::moveChild(int from, int to) {
	items.move(from, to);
}

DfxQBaseNode *DfxLayerItem::getImportParent() {
	return importParent;
}

void DfxLayerItem::setImportParent(DfxQBaseNode *imp) {
	importParent = imp;
}

void DfxLayerItem::addImportItem(DfxLayerItem *it) {
	importItems.push_back(it);
}

void DfxLayerItem::removeImportItem(DfxLayerItem *it) {

	size_t counter = -1;

	for (auto ii : importItems) {
		if (ii->getID() == it->getID()) {
			counter++;
			break;
		}
		counter++;
	}

	if (counter != -1) {
		importItems.remove(counter);
	}
}

QVector<DfxLayerItem *> DfxLayerItem::getImportList() {
	return importItems;
}

bool DfxLayerItem::isImportNode() {
	return importNode;
}

void DfxLayerItem::setImportNode(bool val) {
	importNode = val;
}

bool DfxLayerItem::isTop() {
	return top;
}

void DfxLayerItem::setIsTop(bool tp) {
	top = tp;
}
