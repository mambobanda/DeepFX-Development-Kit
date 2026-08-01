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
#include "DfxQNodeEditorCommon.h"
#include "DfxQComputeNodeEnums.h"


class DfxQComputeErrorIcon : public QGraphicsPathItem {
private:

	float _radius;
	static QSvgRenderer *renderer;
	QGraphicsSvgItem *icon;
	QPainterPath _circle;

	DfxFluentNodeEditor *_editor;
	DfxQListenerItem *_item;

	DfxQComputeNode *_computeNodePtr;
private:

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:
	enum { Type = DfxQNodeEnums::COMPUTE_ERROR_ICON };

	DfxQComputeErrorIcon(float radius = 1.f, QGraphicsItem *parent = nullptr);
	~DfxQComputeErrorIcon();

	void setEditorPtr(DfxFluentNodeEditor *);

	void setItemPtr(DfxQListenerItem *);

	int type() const;

	void setComputePtr(DfxQComputeNode *_computeNodePtr);

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};