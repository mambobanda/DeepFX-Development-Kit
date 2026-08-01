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

#include "DfxWorkGrid.h"


DfxWorkGrid *DfxWorkGrid::_instance = nullptr;

DfxWorkGrid::DfxWorkGrid() {
	setWidth(10);
	setHeight(10);
	setSubgrid(10);
	setSpacing(10);
	centerAtOrigin();

	
}

DfxWorkGrid::~DfxWorkGrid() {

}

DfxWorkGrid *DfxWorkGrid::getInstance() {
	
	if (_instance == nullptr) {
		_instance = new DfxWorkGrid;
	}

	return _instance;
}

void DfxWorkGrid::centerAtOrigin() {
	setOriginX(-(getWidth() * getSpacing() * 0.5f));
	setOriginY(0.f);
	setOriginZ(-(getHeight() * getSpacing() * 0.5f));
}