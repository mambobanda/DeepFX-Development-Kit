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

#include "DfxCacheTrack.h"


DfxCacheTrack::DfxCacheTrack() {
	_active = true;
	_label = "track";
	_path = "";

	_offset = 0;
	_file = nullptr;
	_cacheRecorder = nullptr;

	_cachePlayer = new DfxCachePlayer;
}

DfxCacheTrack::~DfxCacheTrack() {
	delete _cachePlayer;
}

bool DfxCacheTrack::isActive() {
	return _active;
}

void DfxCacheTrack::setActive(bool val) {
	_active = val;
}

std::string DfxCacheTrack::label() {
	return _label;
}

void DfxCacheTrack::setLabel(std::string lbl) {
	_label = lbl;
}

bool DfxCacheTrack::emptyPath() {
	return _path.empty();
}

std::string DfxCacheTrack::path() {
	return _path;
}

void DfxCacheTrack::setPath(std::string pth) {
	_path = pth;
}

bool DfxCacheTrack::beginRecordingCache(int startingFrame) {
	if (_file == nullptr && !_path.empty()) {

		_file = new xvault::h5::h5File();
		if (!_file->open(_path, xvault::io::OPEN)) {
			DfxGlobal::console::println("Failed to create cache file: " + _file->errorMsg());
			delete _file;
			_file = nullptr;

			return false;
		}

		_cacheRecorder = new DfxCacheRecorder(_file);
		_cacheRecorder->startingFrame = startingFrame;
	}

	return true;
}

void DfxCacheTrack::endRecordingCache() {
	if (_file != nullptr) {

		if (_cacheRecorder != nullptr) {
			_cacheRecorder->finalize();
		}

		if (!_file->close()) {
			DfxGlobal::console::println("Failed to close track cache file: " + _file->errorMsg());
		}

		delete _file;
		_file = nullptr;

		delete _cacheRecorder;
		_cacheRecorder = nullptr;
	}
}

void DfxCacheTrack::recordFrame() {
	_cacheRecorder->step();
}

int DfxCacheTrack::offset() {
	return _offset;
}

void DfxCacheTrack::setOffset(int os) {
	_offset = os;
}

void DfxCacheTrack::takeSnapshot(int frame) {
	_cachePlayer->takeSnapshot(_path, frame, _offset);
}

DfxCacheSnapshot *DfxCacheTrack::snapshot() {
	return _cachePlayer->snapshot();
}

void DfxCacheTrack::clearAll() {
	_path = "";
	_cachePlayer->snapshot()->clearAll();
}
