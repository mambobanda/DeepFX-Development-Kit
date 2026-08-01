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

#include <QObject>
#include <vector>
#include "DfxRenderNodeBase.h"

class DfxRenderHub: public QObject {
	Q_OBJECT
private:
	static DfxRenderHub *_instance;

	std::vector<DfxRenderNodeBase*> _renderNode;

	DfxRenderHub();
public:

	virtual ~DfxRenderHub();

	static DfxRenderHub *getInstance();

	void addRenderNode(DfxRenderNodeBase *rnd);
	void removeRenderNode(DfxRenderNodeBase *rnd);
	DfxRenderNodeBase *renderNode(size_t inx);
	int renderNodeCount();

signals:
	void renderNodesUpdated();
};