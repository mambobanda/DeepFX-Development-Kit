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

#include "FlSpaceObserver.h"
#include "FlSpace.h"


namespace fl {
	size_t SpaceObserver::ID_GEN = 0x0f;

	SpaceObserver::SpaceObserver() {
		_id = ID_GEN++;
	}

	SpaceObserver::~SpaceObserver() {
		_parent->removeObserver(this);
	}

	void SpaceObserver::setParent(Space *parent) {
		_parent = parent;
	}

	size_t SpaceObserver::id() {
		return _id;
	}

}