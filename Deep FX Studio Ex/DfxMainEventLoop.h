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

#include <DfxGlobal.h>
#include <QObject>
#include <SgRootNode.h>
#include <SgShapeBuffer.h>
#include <SgShapeNode.h>
#include <SgUtil.h>
#include "DfxFluentTracker.h"
#include "DfxObjectHandler.h"
#include "DfxPropertyConduit.h"
#include "DfxSpace.h"
#include "DfxMode.h"
#include "DfxEventManager.h"
#include "DfxSimulationManager.h"
#include "DfxFluentCleaner.h"
#include "DfxCacheTrackManager.h"


class DfxMainEventLoop: public QObject {
	Q_OBJECT
private:
	static DfxMainEventLoop *_instance;

	DfxMainEventLoop();

private:
	bool _initialKeyframeProcessed;

public:
	enum {VIEW_UPDATE = 0, KEY_FRAME_UPDATE = 1};

	virtual ~DfxMainEventLoop();

	static DfxMainEventLoop *getInstance();

	void emitfinshedProcessingEventsSignal();

	void resetInitialState();

public slots:
	void processEvents(int frame, unsigned int type, std::vector<unsigned int> ignore_list = std::vector<unsigned int>());

signals:
	void finshedProcessingEvents();
};