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

#include <QTime>
#include "DfxTimelinePlayTask.h"
#include "DfxViewportSynchronizer.h"


DfxTimelinePlayTask::DfxTimelinePlayTask(int fps) {
	_eventLoop = nullptr;
	_timer = nullptr;
	play = false;
	_stop = false;
	_fps = fps;

	setAutoDelete(true);
}

DfxTimelinePlayTask::~DfxTimelinePlayTask() {
	_stop = true;
	play = false;

}

void DfxTimelinePlayTask::run() {

	nfps = 0;
	if (_fps > 0) nfps = (int)((1.f / (float)_fps) * 1000.f * 0.3f);

	QTime time;
	time.start();
	play = true;
	while (play) {
		if (time.elapsed() > nfps) {
			DfxViewportSynchronizer::prepCounter();
			emit nextFrameTriggered();
			while (!DfxViewportSynchronizer::synchronized()) {
				if (_stop) return;
			}

			time.restart();
		}
	}
}

void DfxTimelinePlayTask::stop() {
	play = false;
	emit finished();
}

void DfxTimelinePlayTask::nextFrame() {
	if (DfxViewportSynchronizer::synchronized()) {
		DfxViewportSynchronizer::prepCounter();

		emit nextFrameTriggered();
		while (!DfxViewportSynchronizer::synchronized()) {
			_eventLoop->processEvents();
			if (_stop) return;
		}
	}
}
