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

#include "DfxTimelineManager.h"

DfxTimelineManager *DfxTimelineManager::_instance = nullptr;

DfxTimelineManager::DfxTimelineManager() {
	_isPlaying = false;
	_isSimulating = false;
	_simulationPlayTask = nullptr;
	//_playTask = nullptr;

}

DfxTimelineManager::~DfxTimelineManager() {

}

DfxTimelineManager *DfxTimelineManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxTimelineManager;
	}

	return _instance;
}

int DfxTimelineManager::fps() {
	auto _fps = DfxSpace::getInstance()->findAbsolute("~/TIME/fps");
	return _fps->data<int>();
}

void DfxTimelineManager::setFps(int fp) {
	int temp = fp;

	if (temp <= 0) temp = 1;

	auto _fps = DfxSpace::getInstance()->findAbsolute("~/TIME/fps");
	_fps->setData<int>(temp);
	_fps->notifyObservers();
}

int DfxTimelineManager::firstFrame() {
	auto _startFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start");
	return _startFrame->data<int>();
}

void DfxTimelineManager::setFirstFrame(int ff) {
	auto _startFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start");
	_startFrame->setData<int>(ff);
	_startFrame->notifyObservers();
}

int DfxTimelineManager::lastFrame() {
	auto _endFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end");
	return _endFrame->data<int>();
}

void DfxTimelineManager::setLastFrame(int ff) {
	auto _endFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end");
	_endFrame->setData<int>(ff);
	_endFrame->notifyObservers();
}

int DfxTimelineManager::currentFrame() {
	auto _currentFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	return _currentFrame->data<int>();
}

int DfxTimelineManager::lastCurrentFrame() {
	auto _lastCurrentFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/last_frame");
	return _lastCurrentFrame->data<int>();
}

DfxTime DfxTimelineManager::currentTime() {
	DfxTime time;

	time.fps = fps();
	time.frame = currentFrame();
	time.seconds = (double)time.frame / (double)time.fps;

	return time;
}

void DfxTimelineManager::addFpsObserver(fl::Observer *obs) {
	auto _fps = DfxSpace::getInstance()->findAbsolute("~/TIME/fps");
	_fps->addObserver(obs);
}

void DfxTimelineManager::addCurrentFrameObserver(fl::Observer *obs) {
	auto _cFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	_cFrame->addObserver(obs);
}

void DfxTimelineManager::addLastFrameObserver(fl::Observer *obs) {
	auto _lFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/last_frame");
	_lFrame->addObserver(obs);
}

void DfxTimelineManager::addFrameStartObserver(fl::Observer *obs) {
	auto _fstart = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start");
	_fstart->addObserver(obs);
}

void DfxTimelineManager::addFrameEndObserver(fl::Observer *obs) {
	auto _fend = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end");
	_fend->addObserver(obs);
}

fl::Any *DfxTimelineManager::fpsAny() {
	auto _fps = DfxSpace::getInstance()->findAbsolute("~/TIME/fps");
	return _fps;
}

fl::Any *DfxTimelineManager::currentFrameAny() {
	auto _cFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	return _cFrame;
}

fl::Any *DfxTimelineManager::lastFrameAny() {
	auto _lFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/last_frame");
	return _lFrame;
}

fl::Any *DfxTimelineManager::frameStartAny() {
	auto _fstart = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start");
	return _fstart;
}

fl::Any *DfxTimelineManager::frameEndAny() {
	auto _fend = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end");
	return _fend;
}

void DfxTimelineManager::goToFrame(int ff) {	
	stop();

	if (ff == firstFrame()) {
		if (DfxMode::getInstance()->animationMode() != DfxMode::AnimationMode::SIMULATION) {
			DfxMainEventLoop::getInstance()->resetInitialState();
		}
	}

	auto currentFrameAny = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	auto lastFrameAny = DfxSpace::getInstance()->findAbsolute("~/TIME/last_frame");

	lastFrameAny->setData<int>(currentFrameAny->data<int>());
	currentFrameAny->setData<int>(ff);
	currentFrameAny->notifyObservers();
}

void DfxTimelineManager::goToFrameNoInterrupt(int ff) {

	if (ff == firstFrame()) {
		if (DfxMode::getInstance()->animationMode() != DfxMode::AnimationMode::SIMULATION) {
			DfxMainEventLoop::getInstance()->resetInitialState();
		}
	}

	auto currentFrameAny = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	auto lastFrameAny = DfxSpace::getInstance()->findAbsolute("~/TIME/last_frame");

	lastFrameAny->setData<int>(currentFrameAny->data<int>());
	currentFrameAny->setData<int>(ff);
	currentFrameAny->notifyObservers();
}


void DfxTimelineManager::goToNextFrame() {
	stop();

	auto cframe = currentFrame();
	auto lframe = lastFrame();

	if (cframe == lframe) goTofirstFrame();
	else goToFrame(cframe + 1);
}

void DfxTimelineManager::goToNextFrameNoInterrupt() {
	auto cframe = currentFrame();
	auto lframe = lastFrame();

	if (cframe == lframe) goTofirstFrameNoInterrupt();
	else goToFrameNoInterrupt(cframe + 1);
}

void DfxTimelineManager::goToPreviousFrame() {
	stop();

	auto cframe = currentFrame();
	auto fframe = firstFrame();

	if (cframe == fframe) goToLastFrame();
	else goToFrame(cframe - 1);
}

void DfxTimelineManager::goToPreviousFrameNoInterrupt() {
	auto cframe = currentFrame();
	auto fframe = firstFrame();

	if (cframe == fframe) goToLastFrameNoInterrupt();
	else goToFrameNoInterrupt(cframe - 1);
}

void DfxTimelineManager::goTofirstFrame() {
	stop();

	goToFrame(firstFrame());
}

void DfxTimelineManager::goTofirstFrameNoInterrupt() {
	goToFrameNoInterrupt(firstFrame());
}

void DfxTimelineManager::goToLastFrame() {
	stop();

	goToFrame(lastFrame());
}

void DfxTimelineManager::goToLastFrameNoInterrupt() {
	goToFrameNoInterrupt(lastFrame());
}

void DfxTimelineManager::play() {

	if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::KEYFRAME) {
		if (!isPlaying()) {
			auto _playTask = new DfxTimelinePlayTask(fps());

			QObject::connect(this, &DfxTimelineManager::stopTriggered, _playTask, &DfxTimelinePlayTask::stop);
			QObject::connect(_playTask, &DfxTimelinePlayTask::finished, this, &DfxTimelineManager::playTaskFinished);
			QObject::connect(_playTask, &DfxTimelinePlayTask::nextFrameTriggered, this, &DfxTimelineManager::goToNextFrameNoInterrupt);

			QThreadPool::globalInstance()->start(_playTask);

			setIsPlaying(true);

			DfxMode::getInstance()->setPlayMode(DfxMode::PlayMode::PLAYING);

			emit playTriggered();
		}
	}
	else if(DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::SIMULATION) {
		if (!isPlaying() && _simulationPlayTask == nullptr) {
			_simulationPlayTask = new DfxTimelineSimulationPlayTask(fps());

			QObject::connect((DfxTimelineSimulationPlayTask*)_simulationPlayTask, &DfxTimelineSimulationPlayTask::finished, this, &DfxTimelineManager::playTaskFinished);
			QObject::connect((DfxTimelineSimulationPlayTask*)_simulationPlayTask, &DfxTimelineSimulationPlayTask::nextFrameTriggered, this, &DfxTimelineManager::goToNextFrameNoInterrupt);
			QObject::connect((DfxTimelineSimulationPlayTask*)_simulationPlayTask, &DfxTimelineSimulationPlayTask::goToFrameTriggered, this, &DfxTimelineManager::goToFrameNoInterrupt);

			if (DfxSimulationManager::getInstance()->needsReset()) {
				DfxSimulationManager::getInstance()->setStepsCovered(0);
				DfxSimulationManager::getInstance()->forceReset(false);

				DfxMainEventLoop::getInstance()->resetInitialState();
			}
			
			_simulationPlayTask->start();

			setIsPlaying(true);
			setIsSimulating(true);

			DfxMode::getInstance()->setPlayMode(DfxMode::PlayMode::PLAYING);

			emit playTriggered();
		}
	}
}

void DfxTimelineManager::recordCache() {

	int fframe = firstFrame();
	int lframe = lastFrame();

	if ((lframe - fframe) > 0 && !DfxCacheTrackManager::getInstance()->tracks().empty()) {

		auto track = DfxCacheTrackManager::getInstance()->tracks()[0];

		if (track->path().empty()) {
			QSettings settings;
			QString initialDir = settings.value("io/recordCache", QDir::homePath()).toString();

			auto fileName = QFileDialog::getSaveFileName(nullptr, tr("Record Cache"), initialDir, tr("Cache Files (*.flcache)"));

			if (!fileName.isEmpty()) {
				track->setPath(fileName.toStdString());
				settings.setValue("io/recordCache", fileName);
			}
			else {
				DfxGlobal::console::println("Failed to record cache: Unknown cache destination");
				return;
			}
		}

		bool wasActive = false;
		if (track->isActive()) {
			track->setActive(false);
			wasActive = true;
		}

		if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::KEYFRAME) {
			QProgressDialog progress("Recording Cache...", "Stop Recording", 0, lframe - fframe);
			progress.setWindowModality(Qt::WindowModality::ApplicationModal);

			int counter = 0;
			if(!track->beginRecordingCache(fframe)) return;
			for (int i = fframe; i <= lframe; i++) {
				progress.setValue(counter);
				QApplication::processEvents();

				goToFrame(i);

				track->recordFrame();

				if (progress.wasCanceled())
					break;

				counter++;
			}
			track->endRecordingCache();
			progress.setValue(lframe - fframe);

		}
		else if (DfxMode::getInstance()->animationMode() == DfxMode::AnimationMode::SIMULATION) {

			DfxSimulationManager::getInstance()->setStepsCovered(0);
			DfxSimulationManager::getInstance()->forceReset(false);
			DfxMainEventLoop::getInstance()->resetInitialState();

			QProgressDialog progress("Recording Cache...", "Stop Recording", 0, lframe - fframe);
			progress.setWindowModality(Qt::WindowModality::ApplicationModal);

			DfxStageEvaluationInfo info;
			info.currentTime.fps = DfxTimelineManager::getInstance()->fps();

			int counter = 0;
			if (!track->beginRecordingCache(fframe)) return;
			for (int i = fframe; i <= lframe; i++) {
				progress.setValue(counter);
				QApplication::processEvents();

				goToFrame(i);

				DfxMainEventLoop::getInstance()->processEvents(DfxTimelineManager::getInstance()->currentFrame(), DfxMainEventLoop::KEY_FRAME_UPDATE);

				info.currentTime.frame = i;
				info.currentTime.seconds = (double)i / (double)info.currentTime.fps;

				DfxSimulationManager::getInstance()->run(info);

				DfxSimulationManager::getInstance()->setStepsCovered(DfxSimulationManager::getInstance()->stepsCovered() + 1);

				track->recordFrame();
				if (progress.wasCanceled())
					break;

				counter++;
			}
			track->endRecordingCache();
			progress.setValue(lframe - fframe);

		}

		if (wasActive) {
			track->setActive(true);
		}
	}
}

void DfxTimelineManager::stop() {

	if (isPlaying()) {
		if (_simulationPlayTask != nullptr) {
			((DfxTimelineSimulationPlayTask*)_simulationPlayTask)->stop();
			while (_simulationPlayTask->isRunning());
			delete _simulationPlayTask;
			_simulationPlayTask = nullptr;
			DfxMode::getInstance()->setPlayMode(DfxMode::PlayMode::IDLE);
		}
		else {
			emit stopTriggered();
			DfxMode::getInstance()->setPlayMode(DfxMode::PlayMode::IDLE);
		}

		setIsPlaying(false);
		setIsSimulating(false);
	}
}

bool DfxTimelineManager::isPlaying() {
	return _isPlaying;
}

void DfxTimelineManager::setIsPlaying(bool bl) {
	_isPlaying = bl;
}

bool DfxTimelineManager::isSimulating() {
	return _isSimulating;
}

void DfxTimelineManager::setIsSimulating(bool bl) {
	_isSimulating = bl;
}

void DfxTimelineManager::playTaskFinished() {
	setIsPlaying(false);
}