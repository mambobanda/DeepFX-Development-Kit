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

#include "DfxMainLoop.h"
#include "DfxTimelineWidget.h"


DfxMainLoop *DfxMainLoop::instance = nullptr;

void dfxMainLoopEvent() {
	DfxMainLoop::getInstance()->process();
}

DfxMainLoop::DfxMainLoop() {
	current_state = DfxEventLoopState::IDLE_STATE;
}

DfxMainLoop::~DfxMainLoop() {

}

DfxMainLoop *DfxMainLoop::getInstance() {

	if (instance == nullptr) {
		instance = new DfxMainLoop();
	}

	return instance;
}

void DfxMainLoop::setState(DfxEventLoopState st) {
	current_state = st;
}

DfxEventLoopState DfxMainLoop::getState() {
	return current_state;
}

void DfxMainLoop::setTimeline(void *tm) {
	_timeline = tm;
}

void DfxMainLoop::playFunction() {

	DfxTimelineWidget *timeline = (DfxTimelineWidget*)_timeline;

	int mFpsTimer = std::round(1000.f / (float)DfxTime::getInstance()->getFramesPerSec());

	std::vector<DfxEntity*> rootNodes = DfxSceneGraph::getInstance()->getGraphRoots();


	if (DfxSimulationCoreGlobal::getInstance()->getSimulationMode() == DfxSimulationMode::ACTIVE_MODE &&
		!DeepStudioGlobal::getInstance()->allFramesCached()) {

		timeline->nextSimulationFrame();
		DfxSimulationCoreGlobal::getInstance()->initSimulationDomain();

		for (auto root : rootNodes)
			timeline->stepSimulation(root);
	}
	else {
		timeline->nextFrame_();
		DfxGlobal::command::exec("syncInterface");
		QThread::msleep(mFpsTimer);
	}

	DfxGlobal::command::exec("updateViewport");

	for (auto root : rootNodes) delete root;
}

void DfxMainLoop::process() {

	if (current_state == DfxEventLoopState::IDLE_STATE) {

	}
	else if (current_state == DfxEventLoopState::SIMULATE_STATE) {
		playFunction();
	}
	else if (current_state == DfxEventLoopState::PLAY_TIMELINE_STATE) {

	}

}