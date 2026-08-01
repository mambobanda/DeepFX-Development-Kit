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

#include <DfxGlobal.h>
#include <FlCommon.h>
#include <FlAny.h>
#include <FlDataObject.h>
#include <DfxSpace.h>
#include <QSVGRenderer>
#include <QGraphicsSvgItem>
#include "DfxQFluentEditorCrumb.h"
#include "DfxQComputeBaseNode.h"
#include "DfxQComputeSlotColors.h"
#include "DfxQComputeCompoundFunctionalIcon.h"

class DfxQComputeCompoundNode : public DfxQComputeBaseNode {
private:
	QPainterPath combined;
	QPainterPath body;
	QPainterPath header;
	QPainterPath footer;

	static QSvgRenderer *editIconRenderer;
	static QSvgRenderer *functionalIconRenderer;
	QGraphicsSvgItem *editIcon;
	DfxQComputeCompoundFunctionalIcon *functionalIcon;

	DfxFluentNodeEditor *_editor;
protected:
	QGraphicsTextItem *_labelItem;
	std::string _label;
	int _labelPointSize;

	QColor _headerColor;

	bool mousePressed;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:

	enum { Type = DfxQNodeEnums::COMPUTE_COMPOUND_NODE };

	DfxQComputeCompoundNode(QGraphicsItem *parent = nullptr);
	virtual ~DfxQComputeCompoundNode();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void buildNode();
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

	DfxFluentNodeEditor *editor();
	void setEditor(DfxFluentNodeEditor *);

	void setCorrectIconState();

	void setFunctionalOffIcon();
	void setFunctionalOnIcon();

	void updateSlotConnectionPositions() override;
};