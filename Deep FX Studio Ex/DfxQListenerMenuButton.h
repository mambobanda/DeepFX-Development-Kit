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


#include <QApplication.h>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <DfxGlobal.h>
#include "DfxQNodeEditorCommon.h"
#include "DfxFlowHandler.h"
#include "DfxQListenerEnums.h"

class DfxQListenerMenuButton : public QGraphicsPathItem {
private:

	float _radius;
	static QSvgRenderer *renderer;
	QGraphicsSvgItem *icon;
	QPainterPath _box;

	QMenu *menu;
	QAction *addNewFlow;

	DfxQListenerNode *_node;
private:
	
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
	enum { Type = DfxQNodeEnums::LISTENER_EDIT_FLOW };

	DfxQListenerMenuButton(QGraphicsItem *parent = nullptr);
	~DfxQListenerMenuButton();

	DfxQListenerNode *node();
	void setNode(DfxQListenerNode *pnt);

	int type() const;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};