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

#include <QPainterPath>
#include <QGraphicsItem>
#include <QPainter>
#include <QTransform>
#include <QPointF>
#include "DfxQFlowbaseNode.h"
#include "DfxQFlowConnector.h"

class DfxQFlowDecisionNode : public DfxQFlowBaseNode {
private:

	DfxQFlowConnector *_connector_a;
	DfxQFlowConnector *_connector_b;
	DfxQFlowConnector *_connector_input;

	QGraphicsTextItem *_label;
	QPainterPath _shape;

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:

	enum { Type = DfxQNodeEnums::FLOW_DECISION_NODE };

	DfxQFlowDecisionNode(Avoid::Router *rt, QGraphicsItem *parent = nullptr);
	~DfxQFlowDecisionNode();

	int type() const override;
	QPointF center() const override;
	void removeFromFlow() override;

	QString label();
	void setLabel(QString);

	QFont labelFont();

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QPointF yesConnectorPos() override;
	QPointF noConnectorPos()  override;
	QPointF inputConnectorPos() override;
};