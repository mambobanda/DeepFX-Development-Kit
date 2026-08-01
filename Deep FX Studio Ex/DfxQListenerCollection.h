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

#include <QGraphicsScene>
#include <QGraphicsLayout>
#include <QGraphicsView>
#include <QRectF>
#include "DfxQFluentCollectionBase.h"
#include "DfxListenerHandler.h"
#include "DfxQListenerNode.h"


class DfxQListenerCollection : public DfxQFluentCollectionBase {
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

	std::vector<DfxQListenerNode*> nodes;

	DfxFluentNodeEditor *_editor;

public:
	DfxQListenerCollection();
	~DfxQListenerCollection();

	void addListener(DfxQListenerNode *);
	DfxQListenerNode *addListener(DfxListenerHandler &);
	void deleteListener(DfxQListenerNode *);
	void removeListener(DfxQListenerNode *);
	
	void setEditorPtr(DfxFluentNodeEditor *ptr);

	void clearAll() override;
	void addExisting(std::string) override;
	std::pair<std::string, unsigned int> typeInfo() override;

};