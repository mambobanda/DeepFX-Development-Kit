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
#include <QRunnable>
#include <QEventLoop>
#include <QTimer.h>
#include <QThread>
#include <QOpenGlFunctions_3_1>
#include "DfxSimulationManager.h"
#include "DfxStageEvaluationInfo.h"
#include "DfxGlobal.h"
#include "DfxSpace.h"
#include "DfxMode.h"
#include "DfxMainEventLoop.h"


class DfxTimelineSimulationPlayTask : public QThread, public QOpenGLFunctions_3_1 {
	Q_OBJECT
private:
	QEventLoop *_eventLoop;
	QTimer *_timer;
	bool _stop;
	bool _busy;
public:
	DfxTimelineSimulationPlayTask(int fps);
	virtual ~DfxTimelineSimulationPlayTask();

	void run() override;

	void processFrame(DfxStageEvaluationInfo &info, bool stepSim = true);
signals:
	void nextFrameTriggered();
	void goToFrameTriggered(int);
	void finished();

public slots:
	void stop();
	void nextFrame();
};