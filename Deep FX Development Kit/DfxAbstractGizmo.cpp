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

#include "DfxAbstractGizmo.h"



DfxAbstractGizmo::DfxGizmoHandle::DfxGizmoHandle() {
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale    = glm::vec3(1.f);
	
	selected = false;
}
	
DfxAbstractGizmo::DfxGizmoHandle::~DfxGizmoHandle() {

}

DfxAbstractGizmo::DfxAbstractGizmo() {
	position = glm::vec3(0);
	rotation = glm::vec3(0);
	scale = glm::vec3(1.f);

	gizmoScale = 1.f;

	initializeOpenGLFunctions();
}

DfxAbstractGizmo::~DfxAbstractGizmo() {

}

uint DfxAbstractGizmo::hashRGB(unsigned char r, unsigned char g, unsigned char b) {
	uint code = 0;

	code += r;
	code = (code << 5) | (code >> 27);
	code += g;
	code = (code << 5) | (code >> 27);
	code += b;

	return code;
}

uint DfxAbstractGizmo::genHandlerID() {
	return 0;
}

void DfxAbstractGizmo::draw(int mode) {
	reposition();
	for (auto handle : handles) {

		glDisable(GL_LIGHTING);
		glDisable(GL_DITHER);
		glShadeModel(GL_FLAT);

		glPushMatrix();

		glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z));
		glMultMatrixf(&transMat[0][0]);

		glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		rotMat *= glm::rotate(glm::mat4(), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
		rotMat *= glm::rotate(glm::mat4(), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

		glMultMatrixf(&rotMat[0][0]);

		glm::mat4 sclMat = glm::scale(glm::mat4(), glm::vec3(scale.x, scale.y, scale.z));
		glMultMatrixf(&sclMat[0][0]);		

		if (mode == DfxAbstractGizmo::NORMAL_MODE) {

			if (handle->selected) {
				glColor3f(handle->selectionColor[0], handle->selectionColor[1], handle->selectionColor[2]);
			}
			else {
				glColor3f(handle->color[0], handle->color[1], handle->color[2]);
			}
		}
		else {
			glColor3f(handle->id[0], handle->id[1], handle->id[2]);
		}

		handle->draw();

		glPopMatrix();

		glEnable(GL_LIGHTING);
		glEnable(GL_DITHER);
		glShadeModel(GL_FLAT);
	}
	
}

float DfxAbstractGizmo::getGizmoScale() {
	return gizmoScale;
}

void  DfxAbstractGizmo::setGizmoScale(float gz) {
	gizmoScale = gz;
}

bool DfxAbstractGizmo::handleHit(uchar a, uchar b, uchar c) {

	for (auto handle : handles) {

		if (handle->id[0] == a &&
			handle->id[1] == b &&
			handle->id[2] == c) {

			return true;
		}
	}

	return false;
}

bool DfxAbstractGizmo::handleHit(uchar *frame, uchar *out, int &pixIndex, uint pixelCount){

	for (auto handle : handles) {

		int counter = 0;
		for (size_t inx = 0; inx < pixelCount; inx++) {
			if (handle->id[0] == frame[counter] &&
				handle->id[1] == frame[counter + 1] &&
				handle->id[2] == frame[counter + 2]) {

				out[0] = handle->id[0];
				out[1] = handle->id[1];
				out[2] = handle->id[2];

				pixIndex = inx;

				return true;
			}
			counter += 4;
		}
	}

	return false;
}

bool DfxAbstractGizmo::handleSelected() {

	for (auto handle : handles) {

		if(handle->selected) return true;
	}

	return false;
}

void DfxAbstractGizmo::selectHandle(uchar a, uchar b, uchar c) {

	for (auto handle : handles) {

		if (handle->id[0] == a &&
			handle->id[1] == b &&
			handle->id[2] == c) {

			handle->selected = true;
		}
	}
}

void DfxAbstractGizmo::clearSelected() {

	for (auto handle : handles) {
		handle->selected = false;
	}
}

DfxAbstractGizmo::DfxGizmoHandle *DfxAbstractGizmo::getSelectedHandle() {
	for (auto handle : handles) {

		if (handle->selected) return handle;
	}

	return nullptr;
}

glm::vec3 DfxAbstractGizmo::getIntersectionPoint() {
	return intersectionPoint;
}

void DfxAbstractGizmo::setIntersectionPoint(glm::vec3 intp) {
	intersectionPoint = intp;
}

DfxViewItem *DfxAbstractGizmo::getViewItem() {
	return itemPtr;
}

void DfxAbstractGizmo::setViewItem(DfxViewItem *vit) {
	itemPtr = vit;
}

glm::vec3 DfxAbstractGizmo::getPosition() {
	return position;
}