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
#include <QSvgRenderer>
#include <QGraphicsSvgItem>
#include <QObject>
#include "DfxQNodeEditorCommon.h"
#include "DfxListenerHandler.h"
#include "DfxQListenerItem.h"
#include "DfxQListenerEnums.h"
#include "DfxQListenerMenuButton.h"

class DfxQListenerNode: public QObject, public QGraphicsPathItem {
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
private:
	std::vector<DfxQListenerItem*> _items;

	QGraphicsTextItem *_label;

	static QSvgRenderer *renderer_icon;
	QGraphicsSvgItem *icon;
	DfxQListenerMenuButton *menu;

	QPainterPath _header;
	QPainterPath _test;

	float _width;
	float _height;

	float xpos;
	float ypos;

	DfxFluentNodeEditor *_editor;

	std::string _listenerPath;
	std::string _name;

	bool initial_layout;
private:
	void layoutUi();
	QFont headerFont();

	void loadTestData();

protected:
	void DfxQListenerNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
	enum { Type = DfxQNodeEnums::LISTENER_NODE };

	DfxQListenerNode(QGraphicsItem *parent = nullptr);
	~DfxQListenerNode();

	std::string listenerPath();
	void setListenerPath(std::string pth);

	

	int type() const;

	void draw();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	
	void addExisting();
	void addItem(DfxQListenerItem*);
	void newItem(DfxFlowHandler &, std::string name);
	void deleteItem(DfxFlowHandler &, bool addExistingItems = true);
	DfxListenerHandler deleteAllItems();
	void removeItem(DfxQListenerItem*);

	void setEditorPtr(DfxFluentNodeEditor *);

	QString labelText();
	void setLabelText(QString);

	void clear();
	void refresh();

public slots:
	void moveFlowUP(std::string path);
	void moveFlowDown(std::string);
};