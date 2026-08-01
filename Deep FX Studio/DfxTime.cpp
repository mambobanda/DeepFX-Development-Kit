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

#include "DfxTime.h"

DfxTime *DfxTime::instance = nullptr;

DfxTime::DfxTime() {

	totalFrames = 150;
	fps = 30;
	currentframe = 1;
}

DfxTime::~DfxTime() {

}

DfxTime *DfxTime::getInstance() {

	if (instance == nullptr) {
		instance = new DfxTime();
	}

	return instance;
}

void DfxTime::setTotalFrames(int f) {
	while (!classLock.try_lock());

	totalFrames = f;

	DfxTimeActionInfo info;
	info.actionCode = DfxTimeActionListener::NEW_FRAME_COUNT_ACTION;
	info.newFrame = currentframe;
	info.currentFrame = currentframe;

	announce(info);

	classLock.unlock();
}

int DfxTime::getTotalFrames() {
	return totalFrames;
}

void DfxTime::setNewFrame(int f) {
	while (!classLock.try_lock());

	DfxTimeActionInfo info;
	info.actionCode = DfxTimeActionListener::NEW_FRAME_ACTION;
	info.newFrame = f;
	info.currentFrame = currentframe;

	currentframe = f;

	announce(info);
	onActionEntityCompute();

	classLock.unlock();
}

int DfxTime::getCurrentFrame() {
	return currentframe;
}

void DfxTime::setFramesPerSec(int f) {
	while (!classLock.try_lock());

	fps = f;

	DfxTimeActionInfo info;
	info.actionCode = DfxTimeActionListener::NEW_FPS_ACTION;
	info.newFrame = currentframe;
	info.currentFrame = currentframe;

	announce(info);

	classLock.unlock();
}

int DfxTime::getFramesPerSec() {
	return fps;
}

void DfxTime::addActionListener(DfxTimeActionListener *act) {
	actionListnerList.push_back(act);
}

void DfxTime::announce(DfxTimeActionInfo info) {

	for (auto action : actionListnerList) {
		action->response(info);
	}
}

void DfxTime::addOnActionEntity(DfxEntity *ent) {
	//while (!classLock.try_lock());

	onActionEntityList.push_back(ent);

	//classLock.unlock();
}

void DfxTime::removeOnActionEntity(size_t instanceid) {
//	while (!classLock.try_lock());

	size_t count = 0;

	for (auto ent : onActionEntityList) {
		if (ent->getInstanceID() == instanceid) {
			onActionEntityList.erase(onActionEntityList.begin() + count);
			return;
		}

		count++;
	}

//	classLock.unlock();
}

void DfxTime::onActionEntityCompute() {

	//while (!classLock.try_lock());

	for (auto ent : onActionEntityList) {
		ent->compute();
	}

	//classLock.unlock();
}