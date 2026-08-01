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

#include <QObject>
#include <SgNode.h>
#include "DfxMainEventUpdateID.h"
#include "DfxMainEventLoop.h"
#include "DfxMode.h"


class DfxUpdater: public QObject {
	Q_OBJECT
private:
	static DfxUpdater *_instance;

	DfxUpdater();
signals:
	void _sceneUpdated();
	void _nodeTransformUpdated(std::vector<deepsg::SgNode*> nodes);
	void _materialUpdated();
public:

	virtual ~DfxUpdater();
	
	static DfxUpdater *getInstance();
	void sceneUpdated(unsigned int updateType = DfxMainEventLoop::VIEW_UPDATE, bool updateKeys=false);
	void nodeTransformUpdated(std::vector<deepsg::SgNode*> nodes);
	void materialUpdated();
};