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

#include <QVector>
#include <QGraphicsScene>
#include <vector>
#include "DfxQBaseNode.h"
#include "DfxQStandardNode.h"
#include "DfxQCompositeNode.h"
#include "DfxQGroupNode.h"

enum class DfxLayerItemType { LAYER_ITEM, NODE_ITEM, NULL_ITEM, COMPOSITE_ITEM };
enum class DfxLayerNodeType { GENERIC_NODE, VIEW_NODE };

class DfxLayerItem {
private:
	static size_t unique_id_gen;
private:
	size_t id;

	std::string name;

	QGraphicsScene *scene;

	DfxQBaseNode *node;
	DfxQBaseNode *importParent;

	DfxLayerItem *parent;
	QVector<DfxLayerItem*> items;
	QVector<DfxLayerItem*> importItems;

	DfxLayerItemType itemType;
	DfxLayerNodeType nodeType;

	size_t oldInstanceID;

	bool top;
	bool linked;
	bool visible;
	bool locked;
	bool importNode;
public:

	DfxLayerItem();
	~DfxLayerItem();

	size_t getID();

	QGraphicsScene *getScene();

	DfxQBaseNode *getNode(size_t);

	QGraphicsScene *getScene(size_t);
	void setScene(QGraphicsScene *);

	void setName(std::string);
	std::string getName();

	bool isLinked();
	bool isVisible();
	bool isLocked();

	void linkUnlink();
	void hideUnhide();
	void hide();
	void unHide();
	void lockUnlock();

	void setLinked(bool val);
	void setHidden(bool val);
	void setLocked(bool val);

	DfxQBaseNode *getNode();
	void setNode(DfxQBaseNode *);


	DfxQBaseNode *getImportParent();
	void setImportParent(DfxQBaseNode *);

	DfxLayerItem* getChild(size_t);
	void addChild(DfxLayerItem*);
	void removeChild(DfxLayerItem*);
	size_t childCount();

	DfxLayerItemType getItemType();

	void DfxLayerItem::setItemType(DfxLayerItemType typ);

	DfxLayerNodeType getNodeType();

	void DfxLayerItem::setNodeType(DfxLayerNodeType typ);

	DfxLayerItem *getParent();
	void setParent(DfxLayerItem *);
	
	size_t childRow(DfxLayerItem *it);

	size_t row();

	size_t getOldInstanceID();

	void setOldInstanceID(size_t);

	void moveChild(int from, int to);

	void addImportItem(DfxLayerItem *);

	void removeImportItem(DfxLayerItem *);

	QVector<DfxLayerItem *> getImportList();

	bool isImportNode();

	bool isTop();

	void setIsTop(bool);

	void setImportNode(bool);
};