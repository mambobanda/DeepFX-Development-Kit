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

#include <QList>
#include "DfxStudioCommon.h"
#include "DfxScene.h"
#include "DfxRoot.h"
#include "DfxQBaseNode.h"
#include "DfxQNodeView.h"
#include "DfxQNodeEditorDock.h"
#include "DfxEnvelopeMap.h"
#include "DfxLayerManager.h"

class DfxSceneGraph {
private:
	static DfxSceneGraph *instance;
	
	DfxSceneGraph();
private:

	DfxQNodeEditorDock *graphUI;
public:
	~DfxSceneGraph();

	void resetSimulation();

	static DfxSceneGraph *getInstance();

	void initGraphUI();
	DfxQNodeEditorDock *getGraphUI();

	std::vector<DfxEntity*> getGraphRoots();

	//void parentUnparentedNodes(DfxEntity *parent);

	//std::vector<DfxQNode *> getNodeList();
	
};