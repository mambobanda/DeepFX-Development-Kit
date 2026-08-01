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

#include "DfxResourcePtr.h"

size_t DfxResourcePtr::ID_GEN = 0xff;

DfxResourcePtr::DfxResourcePtr() {
	_id = ID_GEN++;
	_ready = false;
	_rptr = nullptr;
}

DfxResourcePtr::~DfxResourcePtr() {
	if (_rptr != nullptr) {
		_rptr->releasePointer(this);
	}
}

void DfxResourcePtr::setIsReady(bool val) {
	_ready = val;
}

void DfxResourcePtr::releasePointer(bool autoDelete) {
	DfxResource::releasePointer(_rptr, this, autoDelete);
}

size_t DfxResourcePtr::id() {
	return _id;
}

size_t DfxResourcePtr::resourceId() {
	return _rptr->id();
}

bool DfxResourcePtr::isReady() {
	return _ready;
}