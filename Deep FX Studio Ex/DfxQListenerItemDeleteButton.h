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
#include "DfxQNodeEditorCommon.h"
#include "DfxFlowHandler.h"
#include "DfxQListenerEnums.h"


class DfxQListenerItemDeleteButton : public QGraphicsPathItem {
private:
	float _radius;
	static QSvgRenderer *renderer;
	QGraphicsSvgItem *icon;
	QPainterPath _circle;

	std::string _flowPath;

	DfxQListenerNode *_node;
private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
	enum { Type = DfxQNodeEnums::LISTENER_DELETE_FLOW};

	DfxQListenerItemDeleteButton(float radius = 1.f, QGraphicsItem *parent = nullptr);
	~DfxQListenerItemDeleteButton();

	int type() const;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	DfxQListenerNode *node();
	void setNode(DfxQListenerNode *pnt);

	std::string flowPath();
	void setFlowPath(std::string);
};