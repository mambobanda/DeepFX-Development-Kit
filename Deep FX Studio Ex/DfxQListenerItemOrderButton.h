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
#include <QObject>
#include "DfxQListenerEnums.h"


class DfxQListenerItemOrderButton : public QObject, public QGraphicsPathItem {
	Q_OBJECT
	//Q_INTERFACES(QGraphicsPathItem)
private:
	int _dir;

	static QSvgRenderer *renderer_down;
	static QSvgRenderer *renderer_up;
	QGraphicsSvgItem *icon;
	QPainterPath _box;

	std::string _flowPath;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
	enum { Type = DfxQNodeEnums::LISTENER_ORDER_FLOW };

	DfxQListenerItemOrderButton(int upDown, QGraphicsItem *parent = nullptr);
	~DfxQListenerItemOrderButton();

	int type() const;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	std::string flowPath();
	void setFlowPath(std::string);

signals:
	void buttonClicked(std::string);
};