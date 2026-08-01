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

#include "DfxGizmoEvent.h"

void DfxGizmoEvent::setScreenX(int val) {
	screenX = val;
}

int DfxGizmoEvent::getScreenX() {
	return screenX;
}

void DfxGizmoEvent::setScreenY(int val) {
	screenY = val;
}

int DfxGizmoEvent::getScreenY() {
	return screenY;
}

void DfxGizmoEvent::setScreenRay(DfxRay ray) {
	screenRay = ray;
}

DfxRay DfxGizmoEvent::getScreenRay() {
	return screenRay;
}

void DfxGizmoEvent::setProjectionType(DfxProjectionType typ) {
	projectionType = typ;
}

DfxProjectionType DfxGizmoEvent::getProjectionType() {
	return projectionType;
}

glm::vec3 DfxGizmoEvent::getScenePos() {
	return scenePos;
}

void DfxGizmoEvent::setScenePos(glm::vec3 pos) {
	scenePos = pos;
}

DfxViewportView DfxGizmoEvent::getView() {
	return view;
}

void DfxGizmoEvent::setView(DfxViewportView vw) {
	view = vw;
}
