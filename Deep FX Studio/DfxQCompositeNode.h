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

#include "DfxBaseCompositeNode.h"
#include "DfxQBaseNode.h"


class DfxQCompositeNode : public DfxQBaseNode {
private:

	QGraphicsTextItem *title;
	QGraphicsSvgItem *importIcon;

	int slotSpacing;

	bool ready;
public:

	DfxQCompositeNode(DfxEntity *entity, QGraphicsItem *parent = nullptr);
	virtual ~DfxQCompositeNode();

	void setTitle(QString);

	void setSlotSpacing(int);

	virtual void rebuild() override;

	void rebuildChildCompositeNodes();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	bool isReady();

	void setReady(bool);

	void addSlot(QString label, DfxQNodeSlotType t, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot);

	void addSlot(DfxQNodeSlot *slot);

	void addInputSlot(QString label, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot);

	void addOutputSlot(QString label, QColor c, DfxPropertyID prop, DfxQBaseNode *parent, bool sqSlot);

	DfxQNodeSlot *findSlot(size_t instance_id, size_t id);

	bool hasSlot(size_t instance_id, size_t id);

	void cleanLooseSlots();

	DfxQBaseNode *findQBaseNode(size_t instance_id);
};