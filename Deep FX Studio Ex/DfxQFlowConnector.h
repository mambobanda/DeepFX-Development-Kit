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
#include "DfxQFlowbaseNode.h"


class DfxQFlowConnector : public DfxQFlowBaseNode {
private:
	QPainterPath _shape;
	QGraphicsTextItem *_label;
	uint _labelPos;
	uint _state;
private:
	DfxQFlowBaseNode *_parentNode;
public:

	enum { Type = DfxQNodeEnums::FLOW_CONNECTOR, DIRECT=0, YES=2, NO=4, LEFT=8, RIGHT=10, INPUT=11};


	DfxQFlowConnector(Avoid::Router *rt, QGraphicsItem *parent = nullptr);
	~DfxQFlowConnector();

	QString labelText();
	void setLabelText(QString);

	uint labelPos();
	void setLabelPos(uint );

	uint state();
	void setState(uint st);

	int type() const override;
	QPointF center() const;
	void removeFromFlow() override;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	DfxQFlowBaseNode *parentNode();
	void setParentNode(DfxQFlowBaseNode *);
};