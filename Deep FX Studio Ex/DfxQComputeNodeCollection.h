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
#include "DfxQComputeBaseNode.h"
#include "DfxQComputeNode.h"
#include "DfxQComputePropertyLink.h"
#include "DfxQComputeNodeEnums.h"


class DfxQComputeNodeCollection : public DfxQFluentCollectionBase {
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

	std::vector<DfxQComputeBaseNode*> nodes;

	std::string _computeGraphPath;

	DfxFluentNodeEditor *_editor;
	
public:

	DfxQComputeNodeCollection();
	~DfxQComputeNodeCollection();

	std::string computeGraphPath();
	void setComputeGraphPath(std::string );

	void constructLinks();

	void addNode(DfxQComputeBaseNode *Node);
	void deleteNode(DfxQComputeBaseNode *Node);
	void removeNode(DfxQComputeBaseNode *Node);
	void deleteAllLinks();

	DfxFluentNodeEditor *editor();
	void setEditor(DfxFluentNodeEditor *edt);

	void clearAll() override;
	void addExisting(std::string) override;
	std::pair<std::string, unsigned int> typeInfo() override;

	void restoreCompoundInNodeLinks(DfxQComputeCompoundInNode *);
	void restoreCompoundOutNodeLinks(DfxQComputeCompoundOutNode *);
};