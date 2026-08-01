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
#include <QGraphicsItem>
#include <QGraphicsView>
#include <DfxQBaseNode.h>
#include <DfxEntity.h>
#include <DfxEntityDB.h>
#include <DfxLayerManager.h>
#include <DfxBounds.h>
#include <vector>

class DfxNodeViewCopyPaste {
private:
	struct CopyConnection {
		size_t output;
		size_t input;
		size_t source;
		size_t target;
	};

	struct Copy {
		double graphPosX;
		double graphPosY;
		size_t old_id;
		bool isViewNode;
		bool isImportNode;
		std::string logical_name;
		DfxEntity *entity;
		std::vector<CopyConnection> connections;
	};
private:
	std::vector<Copy*> copies;
	std::vector<DfxQBaseNode*> nodes;
public:

	DfxNodeViewCopyPaste();
	~DfxNodeViewCopyPaste();

	void copySelectedNodes(QGraphicsScene &);
	void cutSelectedNodes();
	void paste(QGraphicsView *);
	void clear();
};