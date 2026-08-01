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

#include "DfxRotateGizmo.h"


static float HandleScale = 0.05f;


static uchar DfxRotateGizmoHandleID[][3] =
{
	{ 0, 255, 0 }, // Y-Axis
	{ 0, 0, 255 }, // Z-Axis
	{ 255, 0, 0 } // X-Axis
};

void makeCircle() {

}

DfxRotateGizmo::DfxRotateGizmo(DfxViewItem *itptr) {
	setViewItem(itptr);

	glm::vec3 itemPos = itptr->bounds().center();
	position = itemPos;

	//setGizmoScale(0.06f);


	DfxGizmoHandle *yAxisHandle = new DfxGizmoHandle();

	yAxisHandle->id[0] = DfxRotateGizmoHandleID[0][0];
	yAxisHandle->id[1] = DfxRotateGizmoHandleID[0][1];
	yAxisHandle->id[2] = DfxRotateGizmoHandleID[0][2];

	yAxisHandle->color[0] = DfxRotateGizmoHandleID[0][0];
	yAxisHandle->color[1] = DfxRotateGizmoHandleID[0][1];
	yAxisHandle->color[2] = DfxRotateGizmoHandleID[0][2];

	yAxisHandle->selectionColor[0] = 255;
	yAxisHandle->selectionColor[1] = 255;
	yAxisHandle->selectionColor[2] = 0;

	yAxisHandle->draw = [this]() {

		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt * 6.0f;
		tlWd = sc * 0.5f;
		float thFactor = 0.96f;

		glPushMatrix();

		glm::mat4 rotMat = getRotationMat4(1);
		glMultMatrixf(&rotMat[0][0]);

		glBegin(GL_QUAD_STRIP);

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*tlHt;
			double y = sin(angle)*tlHt;

			glVertex3f(x, y, tlWd);
			glVertex3f(x, y, -tlWd);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt * thFactor);
			double y = sin(angle)*(tlHt * thFactor);

			glVertex3f(x, y, -tlWd);
			glVertex3f(x, y, tlWd);
			
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			glVertex3f(x, y, -tlWd);
			glVertex3f(x * thFactor, y * thFactor, -tlWd);

		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			glVertex3f(x * thFactor, y * thFactor, tlWd);
			glVertex3f(x, y, tlWd);			

		}

		glEnd();

		glPopMatrix();
	};

	handles.push_back(yAxisHandle);

	DfxGizmoHandle *xAxisHandle = new DfxGizmoHandle();

	xAxisHandle->id[0] = DfxRotateGizmoHandleID[2][0];
	xAxisHandle->id[1] = DfxRotateGizmoHandleID[2][1];
	xAxisHandle->id[2] = DfxRotateGizmoHandleID[2][2];

	xAxisHandle->color[0] = DfxRotateGizmoHandleID[2][0];
	xAxisHandle->color[1] = DfxRotateGizmoHandleID[2][1];
	xAxisHandle->color[2] = DfxRotateGizmoHandleID[2][2];

	xAxisHandle->selectionColor[0] = 255;
	xAxisHandle->selectionColor[1] = 255;
	xAxisHandle->selectionColor[2] = 0;

	setGizmoScale(0.05f);

	xAxisHandle->draw = [this]() {
	
		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt *5.42f;
		tlWd = sc * 0.5f;
		float thFactor = 0.96f;

		glPushMatrix();

		glm::mat4 rotMat = getRotationMat4(0);
		glMultMatrixf(&rotMat[0][0]);
		
		glm::vec3 tmp;
		glBegin(GL_QUAD_STRIP);

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*tlHt;
			double y = sin(angle)*tlHt;

			tmp = DfxGizmoUtil::rotate90Z(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Z(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt * thFactor);
			double y = sin(angle)*(tlHt * thFactor);

			tmp = DfxGizmoUtil::rotate90Z(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Z(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			tmp = DfxGizmoUtil::rotate90Z(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Z(x * thFactor, y * thFactor, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			tmp = DfxGizmoUtil::rotate90Z(x * thFactor, y * thFactor, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Z(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		glEnd();

		glPopMatrix();
	};
	
	handles.push_back(xAxisHandle);

	DfxGizmoHandle *zAxisHandle = new DfxGizmoHandle();

	zAxisHandle->id[0] = DfxRotateGizmoHandleID[1][0];
	zAxisHandle->id[1] = DfxRotateGizmoHandleID[1][1];
	zAxisHandle->id[2] = DfxRotateGizmoHandleID[1][2];

	zAxisHandle->color[0] = DfxRotateGizmoHandleID[1][0];
	zAxisHandle->color[1] = DfxRotateGizmoHandleID[1][1];
	zAxisHandle->color[2] = DfxRotateGizmoHandleID[1][2];

	zAxisHandle->selectionColor[0] = 255;
	zAxisHandle->selectionColor[1] = 255;
	zAxisHandle->selectionColor[2] = 0;

	setGizmoScale(0.05f);

	zAxisHandle->draw = [this]() {

		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt *4.8f;
		tlWd = sc * 0.5f;
		float thFactor = 0.96f;

		glPushMatrix();

		glm::mat4 rotMat = getRotationMat4(2);
		glMultMatrixf(&rotMat[0][0]);

		glm::vec3 tmp;
		glBegin(GL_QUAD_STRIP);

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*tlHt;
			double y = sin(angle)*tlHt;

			tmp = DfxGizmoUtil::rotate90Y(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Y(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt * thFactor);
			double y = sin(angle)*(tlHt * thFactor);

			tmp = DfxGizmoUtil::rotate90Y(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Y(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			tmp = DfxGizmoUtil::rotate90Y(x, y, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Y(x * thFactor, y * thFactor, -tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		for (int i = 0; i <= 30; i++) {
			double angle = 2 * gizPI*i / 30;
			double x = cos(angle)*(tlHt);
			double y = sin(angle)*(tlHt);

			tmp = DfxGizmoUtil::rotate90Y(x * thFactor, y * thFactor, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
			tmp = DfxGizmoUtil::rotate90Y(x, y, tlWd);
			glVertex3f(tmp.x, tmp.y, tmp.z);
		}

		glEnd();

		glPopMatrix();
	};

	handles.push_back(zAxisHandle);
}

DfxRotateGizmo::~DfxRotateGizmo() {

}

uint DfxRotateGizmo::numberOfHandles() {
	return 3;
}

void DfxRotateGizmo::pressed(DfxGizmoEvent evt) {

	DfxGizmoHandle *h = getSelectedHandle();
	glm::vec3 inter = getIntersectionPoint();

	glm::vec3 interPos(0);

	if (h->id[0] == DfxRotateGizmoHandleID[0][0] &&
		h->id[1] == DfxRotateGizmoHandleID[0][1] &&
		h->id[2] == DfxRotateGizmoHandleID[0][2]) {

		interPos.y = evt.getScreenY();
	}

	if (h->id[0] == DfxRotateGizmoHandleID[1][0] &&
		h->id[1] == DfxRotateGizmoHandleID[1][1] &&
		h->id[2] == DfxRotateGizmoHandleID[1][2]) {

		interPos.x = evt.getScreenX();
	}

	if (h->id[0] == DfxRotateGizmoHandleID[2][0] &&
		h->id[1] == DfxRotateGizmoHandleID[2][1] &&
		h->id[2] == DfxRotateGizmoHandleID[2][2]) {

		interPos.x = evt.getScreenX();
	}

	lastOffset = interPos;
}

void DfxRotateGizmo::released(DfxGizmoEvent evt) {

	lastOffset = glm::vec3(0);

	if (getViewItem() != nullptr) {
		getViewItem()->calculateBounds();
	}
}


void DfxRotateGizmo::moving(DfxGizmoEvent evt) {

	DfxGizmoHandle *h = getSelectedHandle();
	glm::vec3 inter = getIntersectionPoint();
	glm::vec3 iRot = getViewItem()->getRotation();

	if (h->id[0] == DfxRotateGizmoHandleID[0][0] &&
		h->id[1] == DfxRotateGizmoHandleID[0][1] &&
		h->id[2] == DfxRotateGizmoHandleID[0][2]) {

		float offset = evt.getScreenY() - lastOffset.y;
		getViewItem()->setRotation(glm::vec3(iRot.x, iRot.y, (iRot.z - (offset * .5f))));
		//rotation = glm::vec3(rotation.x, rotation.y, (rotation.z - (offset * .5f)));

		lastOffset.y = evt.getScreenY();
	}

	if (h->id[0] == DfxRotateGizmoHandleID[1][0] &&
		h->id[1] == DfxRotateGizmoHandleID[1][1] &&
		h->id[2] == DfxRotateGizmoHandleID[1][2]) {

		float offset = evt.getScreenX() - lastOffset.x;
		getViewItem()->setRotation(glm::vec3((iRot.x - (offset * .5f)), iRot.y , iRot.z));
		//rotation = glm::vec3((rotation.x - (offset * .5f)), rotation.y, rotation.z);

		lastOffset.x = evt.getScreenX();
	}

	if (h->id[0] == DfxRotateGizmoHandleID[2][0] &&
		h->id[1] == DfxRotateGizmoHandleID[2][1] &&
		h->id[2] == DfxRotateGizmoHandleID[2][2]) {

		float offset = evt.getScreenX() - lastOffset.x;
		getViewItem()->setRotation(glm::vec3(iRot.x, (iRot.y + (offset * .5f)), iRot.z));
		//rotation = glm::vec3(rotation.x, (rotation.y + (offset * .5f)), rotation.z);

		lastOffset.x = evt.getScreenX();

	}
}

void DfxRotateGizmo::done() {
	for (auto handle : handles) {
		delete handle;
	}
}

void  DfxRotateGizmo::reposition() {
	position = getViewItem()->getPosition();
}

glm::mat4 DfxRotateGizmo::getRotationMat4(unsigned int axis) {
	switch (getViewItem()->getRotationOrder())
	{
	case 0:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, .0f, .0f));
		}
		else if (axis == 1) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, .0f));
			return trans;
		}
		else if (axis == 2) {
			return glm::mat4(1);
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case 1:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			return trans;
		}
		else if (axis == 1) {		
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, .0f, .0f));
		}
		else if (axis == 2) {
			return glm::mat4(1);
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case 2:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			return glm::mat4(1);
		}
		else if (axis == 1) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, 0.0f, .0f));
			return trans;
		}
		else if (axis == 2) {
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, .0f));
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case 3:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			return glm::mat4(1);
		}
		else if (axis == 1) {
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, .0f));
		}
		else if (axis == 2) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			return trans;
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case 4:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().x), glm::vec3(1.f, 0.0f, 0.0f));
			return trans;
		}
		else if (axis == 1) {
			
			return glm::mat4(1);
		}
		else if (axis == 2) {
			
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case 5:
	{
		// x = 2, y = 0, z = 1
		if (axis == 0) {
			return glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
		}
		else if (axis == 1) {

			return glm::mat4(1);
		}
		else if (axis == 2) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(getViewItem()->getRotation().y), glm::vec3(0.f, 1.0f, 0.0f));
			return trans;

		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	default:
		return glm::mat4(1.f);
		break;
	}

	return glm::mat4(1);
}