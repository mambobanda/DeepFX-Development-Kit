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

#include "DfxScaleGizmo.h"

static float HandleScale = 0.05f;


static uchar DfxScaleGizmoHandleID[][3] =
{
	{ 0, 255, 0 }, // Y-Axis
	{ 0, 0, 255 }, // Z-Axis
	{ 255, 0, 0 } // X-Axis
};


DfxScaleGizmo::DfxScaleGizmo(DfxViewItem *itptr) {
	setViewItem(itptr);

	glm::vec3 itemPos = itptr->bounds().center();
	position = itemPos;

	//setGizmoScale(0.06f);


	DfxGizmoHandle *yAxisHandle = new DfxGizmoHandle();

	yAxisHandle->id[0] = DfxScaleGizmoHandleID[0][0];
	yAxisHandle->id[1] = DfxScaleGizmoHandleID[0][1];
	yAxisHandle->id[2] = DfxScaleGizmoHandleID[0][2];

	yAxisHandle->color[0] = DfxScaleGizmoHandleID[0][0];
	yAxisHandle->color[1] = DfxScaleGizmoHandleID[0][1];
	yAxisHandle->color[2] = DfxScaleGizmoHandleID[0][2];

	yAxisHandle->selectionColor[0] = 255;
	yAxisHandle->selectionColor[1] = 255;
	yAxisHandle->selectionColor[2] = 0;

	yAxisHandle->draw = [this]() {

		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt *5.f;
		tlWd = sc * 0.4f;

		glBegin(GL_QUADS);

		// Head 
		glVertex3f(-sc, hdHt + tlHt, -sc);
		glVertex3f(sc, hdHt + tlHt, -sc);
		glVertex3f(sc, tlHt, -sc);
		glVertex3f(-sc, tlHt, -sc);


		glVertex3f(-sc, hdHt + tlHt, sc);
		glVertex3f(-sc, hdHt + tlHt, -sc);
		glVertex3f(-sc, tlHt, -sc);
		glVertex3f(-sc, tlHt, sc);
		

		glVertex3f(sc, hdHt + tlHt, sc);
		glVertex3f(-sc, hdHt + tlHt, sc);
		glVertex3f(-sc, tlHt, sc);
		glVertex3f(sc, tlHt, sc);
			

		glVertex3f(sc, hdHt + tlHt, -sc);
		glVertex3f(sc, hdHt + tlHt, sc);
		glVertex3f(sc, tlHt, sc);
		glVertex3f(sc, tlHt, -sc);
		

		glVertex3f(sc, tlHt, sc);
		glVertex3f(-sc, tlHt, sc);
		glVertex3f(-sc, tlHt, -sc);
		glVertex3f(sc, tlHt, -sc);
		
	
		glVertex3f(sc, hdHt + tlHt, -sc);
		glVertex3f(-sc, hdHt + tlHt, -sc);
		glVertex3f(-sc, hdHt + tlHt, sc);
		glVertex3f(sc, hdHt + tlHt, sc);		

		glEnd();

		// Tail
		glBegin(GL_QUADS);

		glVertex3f(-tlWd, hdHt, tlWd);
		glVertex3f(tlWd, hdHt, tlWd);
		glVertex3f(tlWd, tlHt, tlWd);
		glVertex3f(-tlWd, tlHt, tlWd);

		glVertex3f(tlWd, hdHt, -tlWd);
		glVertex3f(-tlWd, hdHt, -tlWd);
		glVertex3f(-tlWd, tlHt, -tlWd);
		glVertex3f(tlWd, tlHt, -tlWd);

		glVertex3f(-tlWd, hdHt, -tlWd);
		glVertex3f(-tlWd, hdHt, tlWd);
		glVertex3f(-tlWd, tlHt, tlWd);
		glVertex3f(-tlWd, tlHt, -tlWd);

		glVertex3f(tlWd, hdHt, tlWd);
		glVertex3f(tlWd, hdHt, -tlWd);
		glVertex3f(tlWd, tlHt, -tlWd);
		glVertex3f(tlWd, tlHt, tlWd);

		glEnd();
	};

	handles.push_back(yAxisHandle);

	DfxGizmoHandle *xAxisHandle = new DfxGizmoHandle();

	xAxisHandle->id[0] = DfxScaleGizmoHandleID[2][0];
	xAxisHandle->id[1] = DfxScaleGizmoHandleID[2][1];
	xAxisHandle->id[2] = DfxScaleGizmoHandleID[2][2];

	xAxisHandle->color[0] = DfxScaleGizmoHandleID[2][0];
	xAxisHandle->color[1] = DfxScaleGizmoHandleID[2][1];
	xAxisHandle->color[2] = DfxScaleGizmoHandleID[2][2];

	xAxisHandle->selectionColor[0] = 255;
	xAxisHandle->selectionColor[1] = 255;
	xAxisHandle->selectionColor[2] = 0;

	setGizmoScale(0.05f);

	xAxisHandle->draw = [this]() {
		

		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt *5.f;
		tlWd = sc * 0.4f;

		glm::vec3 tmp;
		glBegin(GL_QUADS);

		// Head 
		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
	
		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		glEnd();

		// Tail
		glBegin(GL_QUADS);

		tmp = DfxGizmoUtil::rotate90X(-tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(-tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(-tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90X(tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90X(tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		glEnd();
	};

	handles.push_back(xAxisHandle);

	DfxGizmoHandle *zAxisHandle = new DfxGizmoHandle();

	zAxisHandle->id[0] = DfxScaleGizmoHandleID[1][0];
	zAxisHandle->id[1] = DfxScaleGizmoHandleID[1][1];
	zAxisHandle->id[2] = DfxScaleGizmoHandleID[1][2];

	zAxisHandle->color[0] = DfxScaleGizmoHandleID[1][0];
	zAxisHandle->color[1] = DfxScaleGizmoHandleID[1][1];
	zAxisHandle->color[2] = DfxScaleGizmoHandleID[1][2];

	zAxisHandle->selectionColor[0] = 255;
	zAxisHandle->selectionColor[1] = 255;
	zAxisHandle->selectionColor[2] = 0;

	setGizmoScale(0.05f);

	zAxisHandle->draw = [this]() {

		sc = getGizmoScale();
		hdHt = sc * 4.f;
		tlHt = hdHt *5.f;
		tlWd = sc * 0.4f;

		glm::vec3 tmp;
		glBegin(GL_QUADS);

		// Head 
		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, -sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(sc, hdHt + tlHt, sc);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		glEnd();

		// Tail
		glBegin(GL_QUADS);

		tmp = DfxGizmoUtil::rotate90Z(-tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(-tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(-tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		tmp = DfxGizmoUtil::rotate90Z(tlWd, hdHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, hdHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, tlHt, -tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);
		tmp = DfxGizmoUtil::rotate90Z(tlWd, tlHt, tlWd);
		glVertex3f(tmp.x, tmp.y, tmp.z);

		glEnd();
	};

	handles.push_back(zAxisHandle);
}

DfxScaleGizmo::~DfxScaleGizmo() {

}

uint DfxScaleGizmo::numberOfHandles() {
	return 3;
}

void DfxScaleGizmo::pressed(DfxGizmoEvent evt) {

	DfxGizmoHandle *h = getSelectedHandle();
	glm::vec3 inter = getIntersectionPoint();

	glm::vec3 interPos(0);

	if (h->id[0] == DfxScaleGizmoHandleID[0][0] &&
		h->id[1] == DfxScaleGizmoHandleID[0][1] &&
		h->id[2] == DfxScaleGizmoHandleID[0][2]) {

		interPos
			= DfxGizmoUtil::yxPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);
	}

	if (h->id[0] == DfxScaleGizmoHandleID[1][0] &&
		h->id[1] == DfxScaleGizmoHandleID[1][1] &&
		h->id[2] == DfxScaleGizmoHandleID[1][2]) {

		interPos
			= DfxGizmoUtil::xzPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);
	}

	if (h->id[0] == DfxScaleGizmoHandleID[2][0] &&
		h->id[1] == DfxScaleGizmoHandleID[2][1] &&
		h->id[2] == DfxScaleGizmoHandleID[2][2]) {

		interPos
			= DfxGizmoUtil::yxPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);
	}

	if (evt.getProjectionType() == DfxProjectionType::PERSPECTIVE) {
		lastOffset = interPos;
	}
	else {
		lastOffset = evt.getScenePos();
	}

}

void DfxScaleGizmo::released(DfxGizmoEvent evt) {

	lastOffset = glm::vec3(0);

	if (getViewItem() != nullptr) {
		getViewItem()->calculateBounds();
	}
}


void DfxScaleGizmo::moving(DfxGizmoEvent evt) {

	DfxGizmoHandle *h = getSelectedHandle();
	glm::vec3 inter = getIntersectionPoint();
	glm::vec3 iScl = getViewItem()->getScale();

	if (h->id[0] == DfxScaleGizmoHandleID[0][0] &&
		h->id[1] == DfxScaleGizmoHandleID[0][1] &&
		h->id[2] == DfxScaleGizmoHandleID[0][2]) {

		if (evt.getProjectionType() == DfxProjectionType::PERSPECTIVE) {

			glm::vec3 interPos =
				DfxGizmoUtil::yxPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);

			float offset = interPos.y - lastOffset.y;
			getViewItem()->setScale(glm::vec3(iScl.x, iScl.y + offset, iScl.z));

			lastOffset.y = interPos.y;
		}
		else {

			if (evt.getView() != DfxViewportView::TOP && evt.getView() != DfxViewportView::BOTTOM) {

				glm::vec3 interPos = evt.getScenePos();

				float offset = interPos.y - lastOffset.y;
				getViewItem()->setScale(glm::vec3(iScl.x, iScl.y + offset, iScl.z));

				lastOffset.y = interPos.y;
			}
		}
	}

	if (h->id[0] == DfxScaleGizmoHandleID[1][0] &&
		h->id[1] == DfxScaleGizmoHandleID[1][1] &&
		h->id[2] == DfxScaleGizmoHandleID[1][2]) {

		if (evt.getProjectionType() == DfxProjectionType::PERSPECTIVE) {

			glm::vec3 interPos =
				DfxGizmoUtil::xzPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);

			float offset = interPos.z - lastOffset.z;
			getViewItem()->setScale(glm::vec3(iScl.x, iScl.y, iScl.z + offset));

			lastOffset.z = interPos.z;
		}
		else {

			if (evt.getView() != DfxViewportView::FRONT && evt.getView() != DfxViewportView::BACK) {
				glm::vec3 interPos = evt.getScenePos();

				float offset = interPos.z - lastOffset.z;
				getViewItem()->setScale(glm::vec3(iScl.x, iScl.y, iScl.z + offset));

				lastOffset.z = interPos.z;
			}
		}
	}

	if (h->id[0] == DfxScaleGizmoHandleID[2][0] &&
		h->id[1] == DfxScaleGizmoHandleID[2][1] &&
		h->id[2] == DfxScaleGizmoHandleID[2][2]) {

		if (evt.getProjectionType() == DfxProjectionType::PERSPECTIVE) {

			glm::vec3 interPos =
				DfxGizmoUtil::xzPlaneRayIntersectionPoint(evt.getScreenRay().origin, evt.getScreenRay().direction, inter);

			float offset = interPos.x - lastOffset.x;
			getViewItem()->setScale(glm::vec3(iScl.x + offset, iScl.y, iScl.z));

			lastOffset.x = interPos.x;
		}
		else {

			if (evt.getView() != DfxViewportView::LEFT && evt.getView() != DfxViewportView::RIGHT) {
				glm::vec3 interPos = evt.getScenePos();

				float offset = interPos.x - lastOffset.x;
				getViewItem()->setScale(glm::vec3(iScl.x + offset, iScl.y, iScl.z));

				lastOffset.x = interPos.x;
			}
		}
	}
}

void DfxScaleGizmo::done() {
	for (auto handle : handles) {
		delete handle;
	}
}

void  DfxScaleGizmo::reposition() {
	position = getViewItem()->getPosition();
	rotation = getViewItem()->getRotation();
}