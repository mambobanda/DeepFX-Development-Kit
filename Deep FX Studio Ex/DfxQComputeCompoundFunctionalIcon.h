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

#include <QGraphicsPathItem>
#include <QPainter>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QMessageBox>
#include <DfxGlobal.h>
#include "DfxQNodeEditorCommon.h"
#include "DfxQComputeNodeEnums.h"

class DfxQComputeCompoundFunctionalIcon : public QGraphicsPathItem {
private:

	float _radius;
	static QSvgRenderer *renderer;
	static QSvgRenderer *renderer1;

	QGraphicsSvgItem *icon;
	QPainterPath _circle;

	DfxQComputeCompoundNode *_computeNodePtr;

private:

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
public:
	enum { Type = DfxQNodeEnums::COMPUTE_COMPOUND_FUNCTIONAL_ICON };

	DfxQComputeCompoundFunctionalIcon(QGraphicsItem *parent = nullptr);
	~DfxQComputeCompoundFunctionalIcon();

	void setComputeNodePtr(DfxQComputeCompoundNode *);
	DfxQComputeCompoundNode *setComputeNodePtr();

	void setOffIcon();
	void setOnIcon();

	int type() const;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};