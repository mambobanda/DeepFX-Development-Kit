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

#include "DfxOpenGLCacheManager.h"
#include "DfxOpenGLCacheQueue.h"

DfxOpenGLCacheManager *DfxOpenGLCacheManager::_instance = nullptr;

DfxOpenGLCacheManager::DfxOpenGLCacheManager() {

	//QObject::connect(DfxMode::getInstance(), &DfxMode::shadeModeChanged, this, &DfxOpenGLCacheManager::shadingModedChanged);
	QObject::connect(DfxMode::getInstance(), &DfxMode::modeChanged, this, &DfxOpenGLCacheManager::editModeChanged);
}

DfxOpenGLCacheManager::~DfxOpenGLCacheManager() {

}

DfxOpenGLCacheManager *DfxOpenGLCacheManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxOpenGLCacheManager;
	}

	return _instance;
}

void DfxOpenGLCacheManager::addObjectCache(size_t uid, DfxObjectCache *dbc) {
	_objectCacheMap[uid] = dbc;
}

void DfxOpenGLCacheManager::removeObjectCache(size_t uid) {
	auto itr = _objectCacheMap.find(uid);

	if (itr != _objectCacheMap.end()) {
		_objectCacheMap.erase(itr);
	}
}

DfxObjectCache* DfxOpenGLCacheManager::findObjectCache(size_t uid) {
	auto itr = _objectCacheMap.find(uid);

	if (itr != _objectCacheMap.end()) {
		return itr->second;
	}

	return nullptr;
}

void DfxOpenGLCacheManager::addVolumeCache(size_t uid, DfxVolumeCache *dbc) {
	_volumeCacheMap[uid] = dbc;
}

void DfxOpenGLCacheManager::removeVolumeCache(size_t uid) {
	auto itr = _volumeCacheMap.find(uid);

	if (itr != _volumeCacheMap.end()) {
		_volumeCacheMap.erase(itr);
	}
}

DfxVolumeCache *DfxOpenGLCacheManager::findVolumeCache(size_t uid) {
	auto itr = _volumeCacheMap.find(uid);

	if (itr != _volumeCacheMap.end()) {
		return itr->second;
	}

	return nullptr;
}

//void DfxOpenGLCacheManager::shadingModedChanged() {
//	
//	for (auto p : _cacheMap) {
//		p.second->cacheMesh();
//	}
//
//	DfxGlobal::command::exec("updateScene");
//}

void DfxOpenGLCacheManager::editModeChanged() {

	for (auto p : _objectCacheMap) {
		DfxOpenGLCacheQueue::getInstance()->queueCache(p.second);
	}

	DfxGlobal::command::exec("updateScene");
}