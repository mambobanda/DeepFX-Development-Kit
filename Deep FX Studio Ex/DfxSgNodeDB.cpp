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

#include "DfxSgNodeDB.h"

DfxSqNodeDB *DfxSqNodeDB::_instance = nullptr;

DfxSqNodeDB::DfxSqNodeDB() {

}

DfxSqNodeDB::~DfxSqNodeDB() {

}

DfxSqNodeDB *DfxSqNodeDB::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxSqNodeDB;
	}

	return _instance;
}

void DfxSqNodeDB::addLoadCallback(std::string key, node::SgNodeLoadCallback cb) {
	_loadCallbackMap[key] = cb;
}

node::SgNodeLoadCallback DfxSqNodeDB::loadCallback(std::string key) {
	auto itr = _loadCallbackMap.find(key);

	if (itr != _loadCallbackMap.end()) {
		return itr->second;
	}

	return nullptr;
}

void DfxSqNodeDB::addDeleteCallback(std::string key, node::SgNodeDeleteCallback cb) {
	_deleteCallbackMap[key] = cb;
}

node::SgNodeDeleteCallback DfxSqNodeDB::deleteCallback(std::string key) {
	auto itr = _deleteCallbackMap.find(key);

	if (itr != _deleteCallbackMap.end()) {
		return itr->second;
	}

	return nullptr;
}