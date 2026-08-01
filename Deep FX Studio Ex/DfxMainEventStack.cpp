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

#include "DfxMainEventStack.h"
#include "DfxMainEventProcess.h"

DfxMainEventStack *DfxMainEventStack::_instance = nullptr;

DfxMainEventStack::DfxMainEventStack() {

}


DfxMainEventStack::~DfxMainEventStack() {

}

DfxMainEventStack *DfxMainEventStack::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxMainEventStack;
	}

	return _instance;
}

void DfxMainEventStack::addProcess(DfxMainEventProcess *evt) {
	_updates.push_back(evt);
}

void DfxMainEventStack::removeProcess(DfxMainEventProcess *evt) {
	auto itr = _updates.begin();

	while (itr != _updates.end()) {
		
		if ((*itr)->id() == evt->id()) {
			_updates.erase(itr);
			return;
		}

		itr++;
	}
}

void DfxMainEventStack::processUpdates(int frame, unsigned int updateType, std::vector<unsigned int> ignore_list) {
	auto ritr = _updates.rbegin();

	while (ritr != _updates.rend()) {

		auto rc = std::find(ignore_list.begin(), ignore_list.end(), (*ritr)->id());

		if (rc == ignore_list.end()) {
			(*ritr)->process(frame, updateType);
		}

		ritr++;
	}
}