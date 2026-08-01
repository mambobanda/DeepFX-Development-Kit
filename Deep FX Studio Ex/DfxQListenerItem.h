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
#include <QMenu>
#include <QGraphicsSceneMouseEvent>
#include <QInputDialog>
#include <QObject>
#include "DfxQNodeEditorCommon.h"
#include "DfxQListenerEnums.h"
#include "DfxQListenerItemDeleteButton.h"
#include "DfxQListenerItemEditButton.h"
#include "DfxQListenerItemDeleteButton.h"
#include "DfxQListenerItemOrderButton.h"
#include "DfxQFluentEditorCrumb.h"
#include "DfxFlowHandler.h"


class DfxQListenerItem : public QGraphicsPathItem{
private:
	static size_t ID_GEN;
	size_t id;
private:
	float _width;
	float _height;

	QGraphicsTextItem *_label;
	DfxQListenerItemEditButton  *_editButton;
	DfxQListenerItemDeleteButton *_deleteButton;
	DfxQListenerItemOrderButton *_orderUp;
	DfxQListenerItemOrderButton *_orderDown;

	DfxQListenerNode *_parent;

	QPainterPath _container;

	std::string _flowPath;
	std::string _flowName;

	DfxFluentNodeEditor *_editor;

	QMenu *menu;
	QAction *rename;

	void layoutUi();
	QFont labelFont();
protected:

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
public:
	enum { Type = DfxQNodeEnums::LISTENER_NODE_ITEM };

	DfxQListenerItem(QGraphicsItem *parent = nullptr);
	~DfxQListenerItem();

	size_t uid();

	void setParent(DfxQListenerNode *);

	int type() const;

	std::string flowPath();
	void setFlowPath(std::string listenerPath);

	std::string flowName();
	void setFlowName(std::string pth);

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	float itemWidth();
	void setItemWidth(float);

	float itemHeight();
	void setItemHeight(float);

	QString labelText();
	void setLabelText(QString);

	void setEditorPtr(DfxFluentNodeEditor *);

	float calcWidth();

	DfxQListenerItemOrderButton *orderUp();
	DfxQListenerItemOrderButton *orderDown();
};