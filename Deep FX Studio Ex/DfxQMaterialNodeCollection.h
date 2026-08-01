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

#pragma once

#include <QGraphicsScene>
#include <QGraphicsLayout>
#include <QGraphicsView>
#include <QRectF>
#include "DfxQMaterialBaseNode.h"
#include "DfxQMaterialNode.h"
#include "DfxQMaterialPropertyLink.h"
#include "DfxQMaterialNodeEnums.h"


class DfxQMaterialNodeCollection: public QGraphicsScene {
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

	std::vector<DfxQMaterialNode*> nodes;

	std::string _materialPath;
public:
	DfxQMaterialNodeCollection();
	~DfxQMaterialNodeCollection();

	std::string materialPath();
	void setMaterialPath(std::string);

	void constructLinks();

	void addNode(DfxQMaterialNode *);
	void deleteNode(DfxQMaterialNode *);
	void removeNode(DfxQMaterialNode *);
	void deleteAllLinks();
};