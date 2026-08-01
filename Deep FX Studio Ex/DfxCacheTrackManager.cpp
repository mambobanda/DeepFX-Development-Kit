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

#include "DfxCacheTrackManager.h"


DfxCacheTrackManager *DfxCacheTrackManager::_instance = nullptr;

DfxCacheTrackManager::DfxCacheTrackManager() {
	_model = new DfxQCacheTrackModel;
	_defaultTrack = new DfxCacheTrack;

	_tracks.push_back(_defaultTrack);
}

DfxCacheTrackManager::~DfxCacheTrackManager() {
	delete _model;
}

DfxCacheTrackManager *DfxCacheTrackManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxCacheTrackManager;
	}

	return _instance;
}

DfxQCacheTrackModel *DfxCacheTrackManager::model() {
	return _model;
}

DfxCacheTrack *DfxCacheTrackManager::track(size_t index) {
	return _tracks[index];
}

std::vector<DfxCacheTrack*> &DfxCacheTrackManager::tracks() {
	return _tracks;
}

void DfxCacheTrackManager::addTrack(DfxCacheTrack *track) {
	_tracks.push_back(track);

	emit newTrackInserted();
}

void DfxCacheTrackManager::deleteTrack(size_t index) {
	if (index > 0) {
		delete _tracks[index];
		_tracks.erase(_tracks.begin() + index);
	}

	emit trackRemoved();
}

void DfxCacheTrackManager::clear() {
	while (!_tracks.empty()){
		delete _tracks[0];
		_tracks.erase(_tracks.begin());
	}

	_defaultTrack = new DfxCacheTrack;
	_tracks.push_back(_defaultTrack);

	emit newTrackInserted();
}

void DfxCacheTrackManager::clearCallback() {
	getInstance()->clear();
}