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

#include "DfxSceneEvent.h"

DfxSceneEvent::DfxSceneEvent() {
	rightMouseButton = false;
	middleMouseButton = false;
	leftMouseButton = false;

	eventCode = 0;
}

DfxSceneEvent::~DfxSceneEvent() {

}

void DfxSceneEvent::setRightMouseButton(bool rm) {
	rightMouseButton = rm;
}

bool DfxSceneEvent::getRightMouseButton() {
	return rightMouseButton;
}

void DfxSceneEvent::setMiddleMouseButton(bool mm){
	middleMouseButton = mm;
}

bool DfxSceneEvent::getMiddleMouseButton() {
	return middleMouseButton;
}

void DfxSceneEvent::setLeftMouseButton(bool lm) {
	leftMouseButton = lm;
}

bool DfxSceneEvent::getLeftMouseButton() {
	return leftMouseButton;
}

void DfxSceneEvent::setEventCode(uint code) {
	eventCode = code;
}

uint DfxSceneEvent::getEventCode() {
	return eventCode;
}

void DfxSceneEvent::setKeyLog(DfxKeyLog log) {
	keyLog = log;
}

DfxKeyLog DfxSceneEvent::getKeyLog() {
	return keyLog;
}

void DfxSceneEvent::setScreenRay(DfxRay ray) {
	screenRay = ray;
}

DfxRay DfxSceneEvent::getScreenRay() {
	return screenRay;
}


void DfxSceneEvent::setScreenX(int val) {
	screenX = val;
}

int DfxSceneEvent::getScreenX() {
	return screenX;
}

void DfxSceneEvent::setScreenY(int val) {
	screenY = val;
}

int DfxSceneEvent::getScreenY() {
	return screenY;
}

DfxColorID DfxSceneEvent::getColorID() {
	return colorID;
}

void DfxSceneEvent::setColorID(DfxColorID cid) {
	colorID = cid;
}

void DfxSceneEvent::setProjectionType(DfxProjectionType proj) {
	projectionType = proj;
}

DfxProjectionType DfxSceneEvent::getProjectionType() {
	return projectionType;
}

glm::vec3 DfxSceneEvent::getScenePos() {
	return scenePos;
}

void DfxSceneEvent::setScenePos(glm::vec3 pos) {
	scenePos = pos;
}

DfxViewportView DfxSceneEvent::getView() {
	return view;
}

void DfxSceneEvent::setView(DfxViewportView vw) {
	view = vw;
}