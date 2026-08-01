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

#include "FlObserver.h"
#include "FlAny.h"

namespace fl {
	size_t Observer::ID_GEN = 0x0f;

	Observer::Observer() {
		_id = ID_GEN++;
		_parent = nullptr;
	}

	Observer::~Observer() {
		if (_parent != nullptr) {
			_parent->removeObserver(this);
		}
	}

	void Observer::setParent(Any *parent) {
		_parent = parent;
	}

	size_t Observer::id() {
		return _id;
	}

	bool Observer::deleteLater() {
		return true;
	}

	void Observer::update() {
		update(_parent);
	}
}