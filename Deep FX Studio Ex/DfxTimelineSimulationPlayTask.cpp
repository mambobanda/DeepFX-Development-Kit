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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "DfxTimelineSimulationPlayTask.h"
#include "DfxTimelineManager.h"
#include "DfxUpdater.h"
#include "DfxPropertyConduit.h"
#include "DfxViewportSynchronizer.h"

DfxTimelineSimulationPlayTask::DfxTimelineSimulationPlayTask(int fps) {
	_eventLoop = nullptr;
	_timer = nullptr;
	_stop = false;
	_busy = false;
	//setAutoDelete(true);
}

DfxTimelineSimulationPlayTask::~DfxTimelineSimulationPlayTask() {
	_stop = true;
}

void DfxTimelineSimulationPlayTask::run() {

	_stop = false;

	nextFrame();
}

void DfxTimelineSimulationPlayTask::stop() {
	_stop = true;
	emit finished();
}

void DfxTimelineSimulationPlayTask::nextFrame() {

	DfxStageEvaluationInfo info;

	info.currentTime.fps = DfxTimelineManager::getInstance()->fps();
	auto startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int current_frame = startFrame + DfxSimulationManager::getInstance()->stepsCovered();

	while (!_stop) {
		if (endFrame < current_frame) {
			QThread::msleep(23);
			processFrame(info, false);

			DfxViewportSynchronizer::prepCounter();

			emit nextFrameTriggered();

			while (!DfxViewportSynchronizer::synchronized()) if (_stop) return;
		}
		else {

			info.currentTime.frame = current_frame;
			info.currentTime.seconds = (double)current_frame / (double)info.currentTime.fps;

			processFrame(info);

			DfxSimulationManager::getInstance()->setStepsCovered(DfxSimulationManager::getInstance()->stepsCovered() + 1);

			DfxViewportSynchronizer::prepCounter();

			emit goToFrameTriggered(current_frame++);
			
			while (!DfxViewportSynchronizer::synchronized()) if(_stop) return;
		}
	}

}

void DfxTimelineSimulationPlayTask::processFrame(DfxStageEvaluationInfo &info, bool stepSim) {
	
	//DfxMainEventLoop::getInstance()->processEvents(DfxTimelineManager::getInstance()->currentFrame(), DfxMainEventLoop::KEY_FRAME_UPDATE);
	QMetaObject::invokeMethod(
		DfxMainEventLoop::getInstance(), "processEvents", Qt::QueuedConnection,
		Q_ARG(int, DfxTimelineManager::getInstance()->currentFrame()),
		Q_ARG(unsigned int, DfxMainEventLoop::KEY_FRAME_UPDATE));

	if(stepSim) DfxSimulationManager::getInstance()->run(info);
}
