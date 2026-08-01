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

#include "DfxSelectionMarquee.h"


DfxSelectionMarquee::DfxSelectionMarquee() {
	active = false;

	_startX = 0;
	_startY = 0;
	_endX = 0;
	_endX = 0;

	_fillColor = glm::ivec4(255, 255, 255, 20);
	_strokeColor = glm::ivec4(255, 255, 255, 150);
}

DfxSelectionMarquee::~DfxSelectionMarquee() {

}

int DfxSelectionMarquee::startX() {
	return _startX;
}

void DfxSelectionMarquee::setStartX(int val) {
	_startX = val;
}

int DfxSelectionMarquee::startY() {
	return _startY;
}

void DfxSelectionMarquee::setStartY(int val) {
	_startY = val;
}

int DfxSelectionMarquee::endX() {
	return _endX;
}

void DfxSelectionMarquee::setEndX(int val) {
	_endX = val;
}

int DfxSelectionMarquee::endY() {
	return _endY;
}

void DfxSelectionMarquee::setEndY(int val) {
	_endY = val;
}

bool DfxSelectionMarquee::isActive() {
	return active;
}

void DfxSelectionMarquee::setIsActive(bool b) {
	active = b;
}

int DfxSelectionMarquee::canvasWidth() {
	return _canvasWidth;
}

void DfxSelectionMarquee::setCanvasWidth(int w) {
	_canvasWidth = w;
}

int DfxSelectionMarquee::canvasHeight() {
	return _canvasHeight;
}

void DfxSelectionMarquee::setCanvasHeight(int h) {
	_canvasHeight = h;
}

void DfxSelectionMarquee::setFillColor(glm::ivec4 d) {
	_fillColor = d;
}

glm::ivec4 DfxSelectionMarquee::fillColor() {
	return _fillColor;
}

void DfxSelectionMarquee::setStrokeColor(glm::ivec4 d) {
	_strokeColor = d;
}

glm::ivec4 DfxSelectionMarquee::strokeColor() {
	return _strokeColor;
}
