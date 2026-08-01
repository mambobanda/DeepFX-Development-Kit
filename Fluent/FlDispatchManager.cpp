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

#include "FlDispatchManager.h"
#include "FlDispatcher.h"

namespace fl {

	DispatchManager *DispatchManager::_instance = nullptr;


	DispatchManager::DispatchManager() {

	}

	DispatchManager::~DispatchManager() {

	}

	DispatchManager *DispatchManager::getInstance() {
		if (_instance == nullptr) {
			_instance = new DispatchManager();
		}

		return _instance;
	}

	bool DispatchManager::hasDispatcher(size_t uid) {
		auto itr = _dispatchMap.find(uid);

		if (itr != _dispatchMap.end()) {
			return true;
		}

		return false;
	}

	std::vector<Dispatcher*> DispatchManager::dispatcherList() {
		std::vector<Dispatcher*> output;

		for (auto d: _dispatchMap) {
			output.push_back(d.second);
		}

		return output;
	}

	void DispatchManager::addDispatcher(Dispatcher *dtp) {
		_dispatchMap[dtp->id()] = dtp;
	}

	void DispatchManager::removeDispatcher(size_t uid) {
		_dispatchMap.erase(uid);
	}

	void DispatchManager::invokeDispatcher(size_t uid, Any *any) {
		auto itr = _dispatchMap.find(uid);

		if (itr != _dispatchMap.end()) {
			itr->second->valueChanged(any);
		}
	}
}