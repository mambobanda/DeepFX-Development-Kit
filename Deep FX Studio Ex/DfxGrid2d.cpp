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

#include "DfxGrid2d.h"


DfxGrid2d::DfxGrid2d() {

}

DfxGrid2d::~DfxGrid2d() {

}

int DfxGrid2d::getWidth() {
	return width;
}

void DfxGrid2d::setWidth(int w) {
	width = w;
}

int DfxGrid2d::getHeight() {
	return height;
}

void DfxGrid2d::setHeight(int h) {
	height = h;
}

int DfxGrid2d::getSubgrid() {
	return subgrid;
}

void DfxGrid2d::setSubgrid(int sg) {
	subgrid = sg;
}

float DfxGrid2d::getSpacing() {
	return spacing;
}

void DfxGrid2d::setSpacing(int sp) {
	spacing = sp;
}

float DfxGrid2d::getOriginX() {
	return originX;
}

void DfxGrid2d::setOriginX(float x) {
	originX = x;
}

float DfxGrid2d::getOriginY() {
	return originY;
}
void DfxGrid2d::setOriginY(float y) {
	originY = y;
}

float DfxGrid2d::getOriginZ() {
	return originZ;
}

void DfxGrid2d::setOriginZ(float z) {	
	originZ = z;
}