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

#include <QGraphicsLayout>
#include <QGraphicsView>
#include <libavoid/libavoid.h>
#include <QRectF>
#include "DfxQNodeEditorCommon.h"
#include "DfxQFluentCollectionBase.h"
#include "DfxListenerHandler.h"
#include "DfxQListenerNode.h"


class DfxQFlowCollection : public DfxQFluentCollectionBase {
protected:
	void drawBackground(QPainter *painter, const QRectF &rect);

	std::vector<DfxQListenerNode*> nodes;

	std::string _flowPath;

	DfxFluentNodeEditor *_editor;

	Avoid::Router *_router;
public:
	DfxQFlowCollection();
	~DfxQFlowCollection();

	std::string flowPath();
	void setFlowPath(std::string );

	void setEditorPtr(DfxFluentNodeEditor *ptr);

	void clearAll() override;
	void addExisting(std::string) override;
	std::pair<std::string, unsigned int> typeInfo() override;

	Avoid::Router *router();

	void addNewProcess();
	void addNewDecision();
	void addNewBreak();

	void restoreFlowNode(int);
	void restoreFlowLinks(fl::Flow *);

	DfxQFlowBaseNode *findNodeByFlowID(int fid);
};