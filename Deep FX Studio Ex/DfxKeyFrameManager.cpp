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

#include "DfxKeyFrameManager.h"
#include "DfxKeyFrameInterpolation.h"

DfxKeyFrameManager *DfxKeyFrameManager::_instance = nullptr;

DfxKeyFrameManager::DfxKeyFrameManager() {

}

DfxKeyFrameManager::~DfxKeyFrameManager() {

}

DfxKeyFrameManager *DfxKeyFrameManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxKeyFrameManager;
	}

	return _instance;
}

// Float Values
void DfxKeyFrameManager::addFloatMap(std::string path, float originalValue) {

	auto *anyMap = new fl::AnyMap<int, float>(path, originalValue);
	_floatMap[path] = anyMap;
}

int DfxKeyFrameManager::floatMapSize(std::string path) {
	auto iter = _floatMap.find(path);

	if (iter != _floatMap.end()) {
		return iter->second->size();
	}

	return -1;
}

void DfxKeyFrameManager::setFloatMapValue(std::string path, int frame, float value) {
	auto iter = _floatMap.find(path);

	if (iter != _floatMap.end()) {
		iter->second->insertValue(frame, value);
	}
}

float DfxKeyFrameManager::getFloatMapValue(std::string path, int frame) {
	auto iter = _floatMap.find(path);

	if (iter != _floatMap.end()) {
		return DfxKeyFrameInterpolation::interpolateFloat(frame, iter->second);
	}

	return 0.f;
}

void DfxKeyFrameManager::removeFloatMapValue(std::string path, int frame) {
	auto iter = _floatMap.find(path);

	if (iter != _floatMap.end()) {
		iter->second->removeValue(frame);
	}
}

void DfxKeyFrameManager::removeFloatMap(std::string path) {

	auto iter = _floatMap.find(path);

	if (iter != _floatMap.end()) {
		delete iter->second;
		_floatMap.erase(iter);
	}
}

fl::AnyMap<int, float> *DfxKeyFrameManager::getFloatMap(std::string path) {
	auto itr = _floatMap.find(path);

	if (itr != _floatMap.end()) return itr->second;

	return nullptr;
}

// Int Values
void DfxKeyFrameManager::addIntMap(std::string path, int originalValue) {

	auto *anyMap = new fl::AnyMap<int, int>(path, originalValue);
	_intMap[path] = anyMap;
}

int DfxKeyFrameManager::intMapSize(std::string path) {
	auto iter = _intMap.find(path);

	if (iter != _intMap.end()) {
		return iter->second->size();
	}

	return -1;
}

void DfxKeyFrameManager::setIntMapValue(std::string path, int frame, int value) {
	auto iter = _intMap.find(path);

	if (iter != _intMap.end()) {
		iter->second->insertValue(frame, value);
	}
}

int DfxKeyFrameManager::getIntMapValue(std::string path, int frame) {
	auto iter = _intMap.find(path);

	if (iter != _intMap.end()) {
		return DfxKeyFrameInterpolation::interpolateInt(frame, iter->second);
	}

	return 0;
}

void DfxKeyFrameManager::removeIntMapValue(std::string path, int frame) {
	auto iter = _intMap.find(path);

	if (iter != _intMap.end()) {
		iter->second->removeValue(frame);
	}
}

void DfxKeyFrameManager::removeIntMap(std::string path) {
	auto iter = _intMap.find(path);

	if (iter != _intMap.end()) {
		delete iter->second;
		_intMap.erase(iter);
	}
}

fl::AnyMap<int, int> *DfxKeyFrameManager::getIntMap(std::string path) {
	auto itr = _intMap.find(path);

	if (itr != _intMap.end()) return itr->second;

	return nullptr;
}

// String Values
void DfxKeyFrameManager::addStringMap(std::string path, std::string originalValue) {

	auto *anyMap = new fl::AnyMap<int,std::string> (path, originalValue);
	_stringMap[path] = anyMap;
}

int  DfxKeyFrameManager::stringMapSize(std::string path) {
	auto iter = _stringMap.find(path);

	if (iter != _stringMap.end()) {
		return iter->second->size();
	}

	return -1;
}

void DfxKeyFrameManager::setStringMapValue(std::string path, int frame, std::string value) {
	auto iter = _stringMap.find(path);

	if (iter != _stringMap.end()) {
		iter->second->insertValue(frame, value);
	}
}

std::string DfxKeyFrameManager::getStringMapValue(std::string path, int frame) {
	auto iter = _stringMap.find(path);

	if (iter != _stringMap.end()) {
		return DfxKeyFrameInterpolation::interpolateString(frame, iter->second);
	}

	return std::string("");
}

void DfxKeyFrameManager::removeStringMapValue(std::string path, int frame) {
	auto iter = _stringMap.find(path);

	if (iter != _stringMap.end()) {
		iter->second->removeValue(frame);
	}
}

void DfxKeyFrameManager::removeStringMap(std::string path) {
	auto iter = _stringMap.find(path);

	if (iter != _stringMap.end()) {
		delete iter->second;
		_stringMap.erase(iter);
	}
}

fl::AnyMap<int, std::string> *DfxKeyFrameManager::getStringMap(std::string path) {
	auto itr = _stringMap.find(path);

	if (itr != _stringMap.end()) return itr->second;

	return nullptr;
}