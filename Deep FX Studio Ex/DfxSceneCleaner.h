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

#include <vector>
#include <SgNode.h>
#include <SgRootNode.h>
#include <QObject>
#include "DfxSceneCleanerCommon.h"
#include "DfxSgNodeDB.h"

class DfxSceneCleaner: public QObject {
	Q_OBJECT
private:
	static DfxSceneCleaner *_instance;

	std::vector<SceneCleanerCallback> _cleanCallbackList;

	DfxSceneCleaner();

private:

	bool cleanGraph(deepsg::SgNode *);
public:

	virtual ~DfxSceneCleaner();

	static DfxSceneCleaner *getInstance();

	int addCallback(SceneCleanerCallback cb);
	void removeCallback(int);

	void cleanGraph();
	void clean();

	void reset();

signals:

	void resetSignal();
};