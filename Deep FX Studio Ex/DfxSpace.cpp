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

#include "DfxSpace.h"


DfxSpace *DfxSpace::_instance = nullptr;

DfxSpace::DfxSpace() {

}

DfxSpace::~DfxSpace() {

}
	
DfxSpace *DfxSpace::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxSpace;
	}

	return _instance;
}

