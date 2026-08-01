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
#include <QThreadpool>
#include <QProgressDialog>
#include <filesystem>
#include <QFileDialog>
#include <QSettings>
#include "DfxSpace.h"
#include "DfxTime.h"
#include "DfxTimelinePlayTask.h"
#include "DfxTimelineSimulationPlayTask.h"
#include "DfxCacheTrackManager.h"

namespace filesystem = std::experimental::filesystem;

class DfxTimelineManager: public QObject {
	Q_OBJECT
private:
	static DfxTimelineManager *_instance;

	bool _isPlaying;
	bool _isSimulating;
	//DfxTimelinePlayTask *_playTask;
	QThread *_simulationPlayTask;

	DfxTimelineManager();
public:
	
	virtual ~DfxTimelineManager();

	static DfxTimelineManager *getInstance();

	int fps();
	void setFps(int fp);

	int firstFrame();
	void setFirstFrame(int ff);

	int lastFrame();
	void setLastFrame(int ff);

	int currentFrame();
	int lastCurrentFrame();

	DfxTime currentTime();

	void addFpsObserver(fl::Observer *);
	void addCurrentFrameObserver(fl::Observer *);
	void addLastFrameObserver(fl::Observer *);
	void addFrameStartObserver(fl::Observer *);
	void addFrameEndObserver(fl::Observer *);

	fl::Any *fpsAny();
	fl::Any *currentFrameAny();
	fl::Any *lastFrameAny();
	fl::Any *frameStartAny();
	fl::Any *frameEndAny();

	bool isPlaying();
	void setIsPlaying(bool);
	
	bool isSimulating();
	void setIsSimulating(bool);

signals:
	void playTriggered();
	void stopTriggered();

public slots:

	void goToFrame(int ff);
	void goToFrameNoInterrupt(int ff);
	void goToNextFrame();
	void goToNextFrameNoInterrupt();
	void goToPreviousFrame();
	void goToPreviousFrameNoInterrupt();
	void goTofirstFrame();
	void goTofirstFrameNoInterrupt();
	void goToLastFrame();
	void goToLastFrameNoInterrupt();
	void play();
	void recordCache();
	void stop();

	void playTaskFinished();
};