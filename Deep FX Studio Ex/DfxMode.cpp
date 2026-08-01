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

#include "DfxMode.h"

DfxMode *DfxMode::_instance = nullptr;

DfxMode::DfxMode() {
	_mode = DfxMode::Mode::OBJECT_EDIT;
	_animationMode = DfxMode::AnimationMode::KEYFRAME;
	_playMode = DfxMode::PlayMode::IDLE;
	_timelineMode = DfxMode::TimelineMode::NONE;
}

DfxMode::~DfxMode() {

}

DfxMode *DfxMode::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxMode;
	}

	return _instance;
}

DfxMode::Mode DfxMode::mode() {
	return _mode;
}

void DfxMode::setMode(DfxMode::Mode m) {
	_mode = m;
	emit modeChanged();
}

DfxMode::AnimationMode DfxMode::animationMode() {
	return _animationMode;
}

void DfxMode::setAnimationMode(DfxMode::AnimationMode animMode) {
	_animationMode = animMode;
	emit animationModeChanged();
}

DfxMode::PlayMode DfxMode::playMode() {
	return _playMode;
}

void DfxMode::setPlayMode(DfxMode::PlayMode pMode) {
	_playMode = pMode;
	emit playModeChanged();
}

DfxMode::TimelineMode DfxMode::timelineMode() {
	return _timelineMode;
}

void DfxMode::setTimelineMode(DfxMode::TimelineMode tmode) {
	_timelineMode = tmode;
	emit timelineModeChanged();
}