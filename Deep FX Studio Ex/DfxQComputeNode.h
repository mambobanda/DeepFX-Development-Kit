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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlCommon.h>
#include <FlAny.h>
#include <FlDataObject.h>
#include <DfxSpace.h>
#include "DfxQComputeBaseNode.h"
#include "DfxQComputeErrorIcon.h"
#include "DfxQComputeSlotColors.h"

class DfxQComputeNode : public DfxQComputeBaseNode {
private:

	QPainterPath combined;
	QPainterPath body;
	QPainterPath header;
	QPainterPath footer;

	DfxQComputeErrorIcon *errorIcon;

protected:

	QGraphicsTextItem *_labelItem;
	std::string _label;
	int _labelPointSize;

	QColor _headerColor;

	std::string _dataObjectPath;

	bool mousePressed;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:

	enum { Type = DfxQNodeEnums::COMPUTE_NODE };

	DfxQComputeNode(QGraphicsItem *parent = nullptr);
	virtual ~DfxQComputeNode();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void buildNode(fl::Any *any);
	void addSlot(fl::Any *any, size_t index);
	void eraseSlot(size_t index);
	void insertSlots();
	void removeSlots();
	int slotIndex(std::string path);
	void updateLinkPositions();

	void clear();

	std::string label();
	void setLabel(std::string str);

	int labelPointSize();
	void setLabelPointSize(int);

	int type() const override;

	QFont labelFont();

	QColor headerColor();
	void setHeaderColor(QColor cl);

	std::string dataObjectPath();
	void setDataObjectPath(std::string);

	void updateSlotConnectionPositions() override;

};