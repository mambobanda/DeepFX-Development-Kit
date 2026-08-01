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

#include <QDebug>
#include <glm/gtx/matrix_decompose.hpp>
#include "EulerAngles.h"
#include "DfxCamera.h"
#include "DfxScenePropRegistry.h"
#include "DfxViewportVersion01.h"

static bool offscreen = false;
float colors[][3] =
{ { 1.0, 0.0, 0.0},
  { 0.0, 1.0, 0.0},
  { 0.0, 0.0, 1.0} };

int colorIndex = 0;

DfxViewportVersion01::DfxViewportVersion01() {

	setMinimumSize(400, 500);

	leftMouseBtnPressed   = false;
	middleMouseBtnPressed = false;
	rightMouseBtnPressed  = false;

	gridVisible = true;
	backfaceVisible = false;
	
	glm::ivec2 lastMousePosition = glm::ivec2(0);
 
	perspViewInfo.viewPhi	= -37.75;
	perspViewInfo.viewTheta	= -27;
	perspViewInfo.viewLamda = 0.f;
	perspViewInfo.viewDepth	= -8.0;
	perspViewInfo.viewPanX	= 0.0f;
	perspViewInfo.viewPanY	= 0.0f;

	topViewInfo.viewPhi	  = 0.f;
	topViewInfo.viewTheta = 0.f;
	topViewInfo.viewDepth = 3.0f;
	topViewInfo.viewPanX  = 0.0f;
	topViewInfo.viewPanY  = 0.0f;

	bottomViewInfo.viewPhi = 0.f;
	bottomViewInfo.viewTheta = 0.f;
	bottomViewInfo.viewDepth = 3.0f;
	bottomViewInfo.viewPanX = 0.0f;
	bottomViewInfo.viewPanY = 0.0f;

	leftViewInfo.viewPhi = 0.f;
	leftViewInfo.viewTheta = 0.f;
	leftViewInfo.viewDepth = 3.0f;
	leftViewInfo.viewPanX = 0.0f;
	leftViewInfo.viewPanY = 0.0f;

	rightViewInfo.viewPhi = 0.f;
	rightViewInfo.viewTheta = 0.f;
	rightViewInfo.viewDepth = 3.0f;
	rightViewInfo.viewPanX = 0.0f;
	rightViewInfo.viewPanY = 0.0f;

	frontViewInfo.viewPhi = 0.f;
	frontViewInfo.viewTheta = 0.f;
	frontViewInfo.viewDepth = 3.0f;
	frontViewInfo.viewPanX = 0.0f;
	frontViewInfo.viewPanY = 0.0f;

	backViewInfo.viewPhi = 0.f;
	backViewInfo.viewTheta = 0.f;
	backViewInfo.viewDepth = 3.0f;
	backViewInfo.viewPanX = 0.0f;
	backViewInfo.viewPanY = 0.0f;

	orthoMat = glm::mat4(1.f);
	perspectiveMat = glm::mat4(1.f);
	viewMat = glm::mat4(1.f);

	renderMode = DfxViewportRenderMode::SOLID;

	bgColorTop =  glm::vec3(0.1f, 0.1f, 0.1f);
	bgColorBottom = glm::vec3(0.2f, 0.2f, 0.2f);

	gridHalfSize = 7;
	gridUnit     = .5f;

	gizmoRatio = 0.0075;

	currentView = DfxViewportView::PERSPECTIVE;

	menus = new DfxViewportMenus(this);
	menus->setCurrentViewText("Perspective");

	viewState = DfxViewportState::NO_STATE;

	this->setFocusPolicy(Qt::StrongFocus);

	keyevent_key = Qt::NoButton;
	mouseevent_button = Qt::NoButton;

	installEventFilter(this);

	// View Renders
	QObject::connect(menus->perspective, &QAction::triggered,
		this, &DfxViewportVersion01::perspectiveView);

	QObject::connect(menus->top, &QAction::triggered,
		this, &DfxViewportVersion01::topView);

	QObject::connect(menus->bottom, &QAction::triggered,
		this, &DfxViewportVersion01::bottomView);

	QObject::connect(menus->left, &QAction::triggered,
		this, &DfxViewportVersion01::leftView);

	QObject::connect(menus->right, &QAction::triggered,
		this, &DfxViewportVersion01::rightView);

	QObject::connect(menus->front, &QAction::triggered,
		this, &DfxViewportVersion01::frontView);

	QObject::connect(menus->back, &QAction::triggered,
		this, &DfxViewportVersion01::backView);

	QObject::connect(menus->cameraFromView, &QAction::triggered,
		this, &DfxViewportVersion01::cameraFromView);

	QObject::connect(menus->viewModeMenu, &QMenu::aboutToShow,
		this, &DfxViewportVersion01::cameraMenu);

	// Render Menus
	QObject::connect(menus->solid, &QAction::triggered,
		this, &DfxViewportVersion01::switchToSolidRenderMode);

	QObject::connect(menus->solidWire, &QAction::triggered,
		this, &DfxViewportVersion01::switchToSolidWireRenderMode);

	QObject::connect(menus->wire, &QAction::triggered,
		this, &DfxViewportVersion01::switchToWireRenderMode);

	// Grid Menus
	QObject::connect(menus->showHideGrid, &QAction::triggered,
		this, &DfxViewportVersion01::switchShowHideGrid);

	QObject::connect(menus->showHideBackface, &QAction::triggered,
		this, &DfxViewportVersion01::switchShowHideBackface);
}

DfxViewportVersion01::DfxViewportVersion01(QSurfaceFormat &fmt) {

	setFormat(fmt);
	DfxViewportVersion01();
}

DfxViewportVersion01::~DfxViewportVersion01() {

}

void DfxViewportVersion01::reset() {

	perspViewInfo.viewPhi = -37.75;
	perspViewInfo.viewTheta = -27;
	perspViewInfo.viewLamda = 0;
	perspViewInfo.viewDepth = -8.0;
	perspViewInfo.viewPanX = 0.0f;
	perspViewInfo.viewPanY = 0.0f;

	topViewInfo.viewPhi = 0.f;
	topViewInfo.viewTheta = 0.f;
	topViewInfo.viewDepth = 3.0f;
	topViewInfo.viewPanX = 0.0f;
	topViewInfo.viewPanY = 0.0f;

	bottomViewInfo.viewPhi = 0.f;
	bottomViewInfo.viewTheta = 0.f;
	bottomViewInfo.viewDepth = 3.0f;
	bottomViewInfo.viewPanX = 0.0f;
	bottomViewInfo.viewPanY = 0.0f;

	leftViewInfo.viewPhi = 0.f;
	leftViewInfo.viewTheta = 0.f;
	leftViewInfo.viewDepth = 3.0f;
	leftViewInfo.viewPanX = 0.0f;
	leftViewInfo.viewPanY = 0.0f;

	rightViewInfo.viewPhi = 0.f;
	rightViewInfo.viewTheta = 0.f;
	rightViewInfo.viewDepth = 3.0f;
	rightViewInfo.viewPanX = 0.0f;
	rightViewInfo.viewPanY = 0.0f;

	frontViewInfo.viewPhi = 0.f;
	frontViewInfo.viewTheta = 0.f;
	frontViewInfo.viewDepth = 3.0f;
	frontViewInfo.viewPanX = 0.0f;
	frontViewInfo.viewPanY = 0.0f;

	backViewInfo.viewPhi = 0.f;
	backViewInfo.viewTheta = 0.f;
	backViewInfo.viewDepth = 3.0f;
	backViewInfo.viewPanX = 0.0f;
	backViewInfo.viewPanY = 0.0f;


	perspectiveMat = glm::mat4(1.f);
	viewMat = glm::mat4(1.f);

	renderMode = DfxViewportRenderMode::SOLID;

	gridVisible = true;
}

void DfxViewportVersion01::resizeGL(int w, int h) {
	initializeOpenGLFunctions();
	glViewport(0, 0, w, h);

}

void DfxViewportVersion01::rotateViewPort(int currentX, int currentY, int lastX, int lastY) {
	if (currentView == DfxViewportView::PERSPECTIVE) {
		perspViewInfo.viewPhi += (float)(currentX - lastX) / 4.0; perspViewInfo.viewTheta += (float)(lastY - currentY) / 4.0;
	}
}

void DfxViewportVersion01::panViewPort(int currentX, int currentY, int lastX, int lastY) {

	if (currentView == DfxViewportView::PERSPECTIVE) {
		perspViewInfo.viewPanX += ((currentX - lastX)*0.01);
		perspViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::TOP) {
		topViewInfo.viewPanX += ((currentX - lastX)*0.01);
		topViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::BOTTOM) {
		bottomViewInfo.viewPanX += ((currentX - lastX)*0.01);
		bottomViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::LEFT) {
		leftViewInfo.viewPanX += ((currentX - lastX)*0.01);
		leftViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::RIGHT) {
		rightViewInfo.viewPanX += ((currentX - lastX)*0.01);
		rightViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::FRONT) {
		frontViewInfo.viewPanX += ((currentX - lastX)*0.01);
		frontViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
	else if (currentView == DfxViewportView::BACK) {
		backViewInfo.viewPanX += ((currentX - lastX)*0.01);
		backViewInfo.viewPanY += ((currentY - lastY)*0.01);
	}
}

void DfxViewportVersion01::zoomViewPort(int currentY, int lastY) {

	if (currentView == DfxViewportView::PERSPECTIVE) {
		perspViewInfo.viewDepth += ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::TOP) {
		topViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::BOTTOM) {
		bottomViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::LEFT) {
		leftViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::RIGHT) {
		rightViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::FRONT) {
		frontViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
	else if (currentView == DfxViewportView::BACK) {
		backViewInfo.viewDepth -= ((float)(lastY - currentY) / 10.0)* 0.2f;
	}
}

void DfxViewportVersion01::drawBackground() {

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);
	
	glColor3f(bgColorTop.x, bgColorTop.y, bgColorTop.z);
	glVertex2f(-1.f, -1.f);
	glVertex2f( 1.f,  -1.f);

	glColor3f(bgColorBottom.x, bgColorBottom.y, bgColorBottom.z);
	glVertex2f(  1.f,  1.f);
	glVertex2f( -1.f,  1.f);

	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void  DfxViewportVersion01::drawObject(DfxObjectItem* ob) {

	if (renderMode == DfxViewportRenderMode::WIRE) {
		glDisable (GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}

	glPushMatrix();

	if (!ob->ignoreLocalTransform()) {

		glm::mat4 transMat = ob->transformationMatrix();
		glMultMatrixf(&transMat[0][0]);
	}

	ob->draw();

	glPopMatrix();

	if (renderMode == DfxViewportRenderMode::WIRE) {
		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
}

void DfxViewportVersion01::drawObjectSelectionMode(DfxObjectItem* ob) {
	glPushMatrix();

	if (!ob->ignoreLocalTransform()) {

		glm::mat4 transMat = ob->transformationMatrix();
		glMultMatrixf(&transMat[0][0]);
	}

	ob->drawSelection();

	glPopMatrix();
}

void DfxViewportVersion01::drawScene() {

	DfxScene *scene = DfxScene::getInstance();
	
	size_t obj_count = scene->itemCount();
	
	for (size_t obj_inx = 0; obj_inx < obj_count; obj_inx++) {

		if (scene->getItem(obj_inx)->getItemType() == DFX_OBJECT_ITEM) {
			if (((DfxObjectItem*)scene->getItem(obj_inx))->isReady()) {

				if (!((DfxObjectItem*)scene->getItem(obj_inx))->isHidden()) {
					drawObject((DfxObjectItem*)scene->getItem(obj_inx));
				}
			}
		}

		if (scene->getItem(obj_inx)->getItemType() == DFX_POINT_ITEM) {

			if (!((DfxViewItem*)scene->getItem(obj_inx))->isHidden()) {
				((DfxPointItem*)scene->getItem(obj_inx))->draw();
			}
		}

		if (scene->getItem(obj_inx)->getItemType() == DFX_CUSTOM_ITEM) {

			if (scene->getItem(obj_inx)->UniqueName().compare("2F2X252X_DFX_CAMERA") == 0 ) {

				if (currentView != DfxViewportView::CAMERA) {
					if (!((DfxViewItem*)scene->getItem(obj_inx))->isHidden()) {
						scene->getItem(obj_inx)->draw();
					}
				}
			}
			else {
				if (!((DfxViewItem*)scene->getItem(obj_inx))->isHidden()) {
					scene->getItem(obj_inx)->draw();
				}
			}
		}
	}
}

void DfxViewportVersion01::drawSceneSelectionMode() {
	DfxScene *scene = DfxScene::getInstance();

	size_t obj_count = scene->itemCount();

	makeCurrent();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraTransform();

	for (size_t obj_inx = 0; obj_inx < obj_count; obj_inx++) {

		if (scene->getItem(obj_inx)->getItemType() == DFX_OBJECT_ITEM) {
			if (((DfxObjectItem*)scene->getItem(obj_inx))->isReady()) {

				if (!((DfxObjectItem*)scene->getItem(obj_inx))->isHidden()) {
					drawObjectSelectionMode((DfxObjectItem*)scene->getItem(obj_inx));
				}
			}
		}

		if (scene->getItem(obj_inx)->getItemType() == DFX_POINT_ITEM) {

			if (!((DfxViewItem*)scene->getItem(obj_inx))->isHidden()) {
				((DfxPointItem*)scene->getItem(obj_inx))->drawSelection();
			}
		}

		if (scene->getItem(obj_inx)->getItemType() == DFX_CUSTOM_ITEM) {

			if (!((DfxViewItem*)scene->getItem(obj_inx))->isHidden()) {
				scene->getItem(obj_inx)->drawSelection();
			}
		}
	}

	glDisable(GL_DEPTH_TEST);
}

void DfxViewportVersion01::drawGizmos() {
	DfxScene *scene = DfxScene::getInstance();
	DfxAbstractGizmo *gizmo = scene->getActiveGizmo();

	if (gizmo != nullptr) {
		if (currentView == DfxViewportView::PERSPECTIVE) {
			gizmo->setGizmoScale(gizmoRatio * glm::distance(gizmo->getPosition(), cameraPosition));
		}
		else {
			gizmo->setGizmoScale(gizmoRatio * glm::distance(gizmo->getPosition(), orthoCameraPosition));
		}

		gizmo->draw(DfxAbstractGizmo::NORMAL_MODE);
	}
}

glm::vec3 extractCameraPosition(const glm::mat4 &vmat) {
	glm::mat3 rotMat(vmat);
	glm::vec3 d(vmat[3]);

	return -d * rotMat;
}

void DfxViewportVersion01::preprocess() {

	DfxScene *scene = DfxScene::getInstance();

	size_t obj_count = scene->itemCount();

	for (size_t obj_inx = 0; obj_inx < obj_count; obj_inx++) {

		if (scene->getItem(obj_inx)->preprocessViewItem()) {
			scene->getItem(obj_inx)->preprocess();
		}
	}
}

void DfxViewportVersion01::cameraTransform() {

	int w = size().width();
	int h = size().height();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspectRatio = (GLfloat)w / (GLfloat)h;

	if (currentView == DfxViewportView::PERSPECTIVE) {

		perspectiveMat = glm::perspective(45.0f, (float)w / h, 0.05f, 500.0f);
		glMultMatrixf(&perspectiveMat[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(viewPanX, viewPanY, viewDepth));
		cameraPosition = glm::vec3(0);

		viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(perspViewInfo.viewPanX, -perspViewInfo.viewPanY, perspViewInfo.viewDepth));

		viewMat *= glm::rotate(glm::radians(-perspViewInfo.viewTheta), glm::vec3(1.0f, 0.0f, 0.0f));

		viewMat *= glm::rotate(glm::radians(perspViewInfo.viewPhi), glm::vec3(0.0f, 1.0f, 0.0f));

		viewMat *= glm::rotate(glm::radians(perspViewInfo.viewLamda), glm::vec3(0.0f, 0.0f, 1.0f));

		glMultMatrixf(&viewMat[0][0]);
		cameraPosition = extractCameraPosition(viewMat);

	}if (currentView == DfxViewportView::CAMERA){

		float fov = 45.0f;

		DfxCamera *cam_t = nullptr;
		float cam_w = 640.f;
		float cam_h = 480.f;

		static float proj[16];

		for (auto cam : DfxScenePropRegistry::getIntstance()->findAllTypes(DfxScenePropRegistry::CAMERA)) {
			if (cam->getInstanceID() == cameraViewInfo.cameraID) {
				cam_t = (DfxCamera*)cam;
			}
		}

		if (cam_t != nullptr) {

			cam_w = *(float*)cam_t->getProperty(DfxCamera::inWidth);
			cam_h = *(float*)cam_t->getProperty(DfxCamera::inHeight);

			float flength = *(float*)cam_t->getProperty(DfxCamera::inFocalLength);
			float wd = *(float*)cam_t->getProperty(DfxCamera::inHoriziontalFOV);
			float hd = *(float*)cam_t->getProperty(DfxCamera::inVerticalFOV);
			
			float dim = std::sqrtf((wd *wd) + (hd *hd));

			fov = 2.f * glm::degrees(std::atan((dim * .56f) / (flength*2.f)));

		}

		auto _proj = glm::perspective(glm::radians(fov), cam_w / cam_h, 0.05f, 500.0f);
		glPushMatrix();
		glMultMatrixf(&_proj[0][0]);
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		glPopMatrix();

		//perspectiveMat = glm::perspective(glm::radians(fov), (float)w / h, 0.05f, 500.0f);
		//glMultMatrixf(&perspectiveMat[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		cameraPosition = glm::vec3(0);

		viewMat = glm::inverse(cameraViewInfo.transform);
		glMultMatrixf(&viewMat[0][0]);
		cameraPosition = extractCameraPosition(viewMat);

		const double _near = proj[11] / (proj[10] - 1.0);
		
		const double nLeft  = _near * (proj[2] - 1.0) / proj[0];
		const double nRight = _near * (1.0 + proj[2]) / proj[0];
		const double nTop   = _near * (1.0 + proj[6]) / proj[5];
		const double nBottom = _near * (proj[6] - 1.0) / proj[5];

		glDisable(GL_LIGHTING);
		glDisable(GL_DITHER);
		glShadeModel(GL_FLAT);
		glLineStipple(1, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(2.0);
		glPushMatrix();
		glMultMatrixf(&cameraViewInfo.transform[0][0]);
		glBegin(GL_LINES);

		glColor3f(1.f, 1.f, 1.f);

		glVertex3f(nLeft, nBottom, -_near);
		glVertex3f(nRight, nBottom, -_near);

		glVertex3f(nRight, nTop, -_near);
		glVertex3f(nLeft, nTop, -_near);

		glVertex3f(nLeft, nTop, -_near);
		glVertex3f(nLeft, nBottom, -_near);

		glVertex3f(nRight, nTop, -_near);
		glVertex3f(nRight, nBottom, -_near);

		glEnd();
		glPopMatrix();

		glEnable(GL_LIGHTING);
		glDisable(GL_LINE_STIPPLE);
		glEnable(GL_LIGHTING);
		glEnable(GL_DITHER);
		glShadeModel(GL_FLAT);

	}
	else  {

		ViewInformation currentViewinfo;

		if (currentView == DfxViewportView::TOP) {
			currentViewinfo = topViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(topViewInfo.viewPanX, -topViewInfo.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (currentView == DfxViewportView::BOTTOM) {
			currentViewinfo = bottomViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(bottomViewInfo.viewPanX, -bottomViewInfo.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (currentView == DfxViewportView::LEFT) {
			currentViewinfo = leftViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(leftViewInfo.viewPanX, -leftViewInfo.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (currentView == DfxViewportView::RIGHT) {
			currentViewinfo = rightViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(rightViewInfo.viewPanX, -rightViewInfo.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (currentView == DfxViewportView::FRONT) {
			currentViewinfo = frontViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(frontViewInfo.viewPanX, -frontViewInfo.viewPanY, 0.0f));
			//viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (currentView == DfxViewportView::BACK) {
			currentViewinfo = backViewInfo;
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(backViewInfo.viewPanX, -backViewInfo.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f));
			viewMat *= glm::rotate(glm::radians(180.f), glm::vec3(1.0f, 0.0f, 0.0f));
			
		}

		if (w <= h) {
			orthoMat = glm::ortho(-currentViewinfo.viewDepth, currentViewinfo.viewDepth,
								  -currentViewinfo.viewDepth / aspectRatio,
								   currentViewinfo.viewDepth / aspectRatio, 
								  -100.f, 100.f);
		}
		else {
			orthoMat = glm::ortho(-currentViewinfo.viewDepth * aspectRatio, currentViewinfo.viewDepth * aspectRatio,
								  -currentViewinfo.viewDepth, currentViewinfo.viewDepth,
								  -100.1f, 100.f);
		}

		glMultMatrixf(&orthoMat[0][0]);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		cameraPosition = glm::vec3();
		orthoCameraPosition = glm::vec3(currentViewinfo.viewDepth * 2.f, currentViewinfo.viewDepth * 2.f, 0.f);;

		glMultMatrixf(&viewMat[0][0]);
		cameraPosition = extractCameraPosition(viewMat);
	}
}

void DfxViewportVersion01::drawGrid() {

	if (!gridVisible) return;

	float gcolor[] = {.7f, .7f, .7f};

	glDisable(GL_LIGHTING);

	float minX = -(gridHalfSize * gridUnit);
	float minZ = -(gridHalfSize * gridUnit);
	float maxX = (gridHalfSize * gridUnit);
	float maxZ = (gridHalfSize * gridUnit);

	glColor3f(gcolor[0], gcolor[1], gcolor[2]);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0);
	
	glBegin(GL_LINES);
	for (uint xindex = 0; xindex <= (gridHalfSize * 2); xindex++) {

		if ((minX + (xindex * gridUnit)) == 0) {
			glColor3f(0.f, 0.f, 1.0f);

			glVertex3f(minX + (xindex * gridUnit), 0.0, minZ);
			glVertex3f(minX + (xindex * gridUnit), 0.0, maxZ);

			glColor3f(gcolor[0], gcolor[1], gcolor[2]);
		}
		else {
			glVertex3f(minX + (xindex * gridUnit), 0.0, minZ);
			glVertex3f(minX + (xindex * gridUnit), 0.0, maxZ);
		}
		
	}

	for (uint yindex = 0; yindex <= (gridHalfSize * 2); yindex++) {

		if ((minX + (yindex * gridUnit)) == 0) {
			glColor3f(1.f, 0.f, .0f);

			glVertex3f(minX, 0.0, minZ + (yindex * gridUnit));
			glVertex3f(maxX, 0.0, minZ + (yindex * gridUnit));

			glColor3f(gcolor[0], gcolor[1], gcolor[2]);
		}
		else {
			glVertex3f(minX, 0.0, minZ + (yindex * gridUnit));
			glVertex3f(maxX, 0.0, minZ + (yindex * gridUnit));
		}
		
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void DfxViewportVersion01::paintGL() {
	initializeOpenGLFunctions();
	if (!offscreen) {

		// preprocess();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawBackground();

		cameraTransform();

		drawGrid();

		drawScene();

		glClear(GL_DEPTH_BUFFER_BIT);

		drawGizmos();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(1.0, 0.0f, 0.0f, 1.f);
	}
}

void DfxViewportVersion01::initializeGL() {

	initializeOpenGLFunctions();

}

QImage *DfxViewportVersion01::capture() {
	auto size = this->size();

	makeCurrent();

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	QOpenGLFramebufferObject *fbo = new QOpenGLFramebufferObject(size.width(), size.height(), format);
	fbo->bind();
	QOpenGLPaintDevice fboPainter(size.width(), size.height());
	QPainter painter(&fboPainter);
	
	paintGL();

	QImage *image = new QImage(fbo->toImage());

	fbo->release();
	delete fbo;
	
	glPopAttrib();
	//byte *pixmap = (byte *)malloc(size.width() * size.height() * 3);

	//makeCurrent();
	//paintGL();

	//glReadBuffer(GL_FRONT);
	////glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	////glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//glReadPixels(0, 0, size.width(), size.width(), GL_RGB, GL_UNSIGNED_BYTE, pixmap);

	//QImage *image = new QImage(pixmap, size.width(), size.height(), QImage::Format::Format_RGB32);

	//free(pixmap);
	
	return image;
}

bool DfxViewportVersion01::eventFilter(QObject *obj, QEvent *event) {

	if (event->type() == QEvent::MouseButtonPress) {
		auto mevent = static_cast<QMouseEvent*>(event);
		mouseevent_button = mevent->button();
	}

	if (event->type() == QEvent::MouseButtonRelease) {
		auto mevent = static_cast<QMouseEvent*>(event);

		if (mouseevent_button == mevent->button()) mouseevent_button = Qt::NoButton;
	}

	if (event->type() == QEvent::KeyPress) {
		auto kevent = static_cast<QKeyEvent*>(event);
		keyevent_key = kevent->key();
	}

	if (event->type() == QEvent::KeyRelease) {
		auto kevent = static_cast<QKeyEvent*>(event);

		if (keyevent_key == kevent->key()) keyevent_key = Qt::NoButton;
	}

	return DfxAbstractViewport::eventFilter(obj, event);
}

void DfxViewportVersion01::mousePressEvent(QMouseEvent *evt) {

	int x = evt->x();
	int y = evt->y();

	int width = this->size().width();
	int height = this->size().height();

	DfxSceneEvent s_evt;
	s_evt.setScreenX(x);
	s_evt.setScreenY(y);
	s_evt.setLeftMouseButton(true);
	s_evt.setEventCode(DfxSceneEvent::MOUSE_PRESS);
	s_evt.setScreenRay(calcScreenRay(x, y, width, height));
	s_evt.setView(currentView);

	lastMousePosition = glm::ivec2(evt->x(), evt->y());

	if (evt->button() == Qt::MouseButton::LeftButton) {
		leftMouseBtnPressed = true;

		if ( DfxScene::getInstance()->getActiveGizmo() != nullptr ) {

			DfxAbstractGizmo *gizmo = DfxScene::getInstance()->getActiveGizmo();

			GLubyte pixel[4];
			GLfloat dz = 0.f;

			makeCurrent();
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			cameraTransform();
			
			if (currentView == DfxViewportView::PERSPECTIVE) {
				gizmo->setGizmoScale(gizmoRatio * glm::distance(gizmo->getPosition(), cameraPosition));
			}
			else {
				gizmo->setGizmoScale(gizmoRatio * glm::distance(gizmo->getPosition(), orthoCameraPosition));
			}

			gizmo->draw(DfxAbstractGizmo::SELECT_MODE);
			glDisable(GL_DEPTH_TEST);

			glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);

			if (gizmo->handleHit(pixel[0], pixel[1], pixel[2])) {			

				glm::vec3 handlepos;

				if (currentView == DfxViewportView::PERSPECTIVE) {
					handlepos = glm::unProject(glm::vec3(x, height - y, dz),
						viewMat, perspectiveMat, glm::vec4(0, 0, width, height));
				}
				else {
					handlepos = glm::unProject(glm::vec3(x, height - y, dz),
						viewMat, orthoMat, glm::vec4(0, 0, width, height));
				}

				s_evt.setScenePos(handlepos);

				gizmo->selectHandle(pixel[0], pixel[1], pixel[2]);
				gizmo->setIntersectionPoint(handlepos);
				DfxScene::getInstance()->newSceneEvent(s_evt);

				viewState = DfxViewportState::GIZMO_SELECTED;
			}

		}
	}

	/*if (viewState == DfxViewportState::NO_STATE) {

		if (evt->button() == Qt::MouseButton::LeftButton) {
			leftMouseBtnPressed = true;

		}

		if (evt->button() == Qt::MouseButton::MiddleButton) {
			middleMouseBtnPressed = true;

		}

		if (evt->button() == Qt::MouseButton::RightButton) {
			rightMouseBtnPressed = true;

		}
	}*/

	update();
}

void DfxViewportVersion01::mouseReleaseEvent(QMouseEvent *evt) {
	
	int x = evt->x();
	int y = evt->y();

	int width = this->size().width();
	int height = this->size().height();

	DfxSceneEvent s_evt;
	s_evt.setScreenX(x);
	s_evt.setScreenY(y);
	s_evt.setLeftMouseButton(true);
	s_evt.setEventCode(DfxSceneEvent::MOUSE_RELEASE);
	s_evt.setScreenRay(calcScreenRay(x, y, width, height));
	s_evt.setView(currentView);
	
	if (currentView == DfxViewportView::PERSPECTIVE) {
		s_evt.setProjectionType(DfxProjectionType::PERSPECTIVE);
	}
	else {
		s_evt.setProjectionType(DfxProjectionType::ORTHOGRAPHIC);
	}


	if (evt->button() == Qt::MouseButton::LeftButton) {
		leftMouseBtnPressed = false;

		if (viewState == DfxViewportState::GIZMO_SELECTED) {

			DfxScene::getInstance()->newSceneEvent(s_evt);
			DfxScene::getInstance()->getActiveGizmo()->clearSelected();
		}

		if (viewState == DfxViewportState::NO_STATE) {

			drawSceneSelectionMode();

			GLubyte pixel[4];
			GLfloat dz = 0.f;

			glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
			glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);

			s_evt.setColorID(DfxColorID(pixel[0], pixel[1], pixel[2]));
			DfxScene::getInstance()->newSceneEvent(s_evt);
		}
	}

	//if (evt->button() == Qt::MouseButton::MiddleButton) {
	//	middleMouseBtnPressed = false;

	//}

	//if (evt->button() == Qt::MouseButton::RightButton) {
	//	rightMouseBtnPressed = false;

	//}
	
	viewState = DfxViewportState::NO_STATE;

	update();
}

void DfxViewportVersion01::mouseMoveEvent(QMouseEvent *evt) {

	int x = evt->x();
	int y = evt->y();

	int width = this->size().width();
	int height = this->size().height();

	DfxSceneEvent s_evt;
	s_evt.setScreenX(x);
	s_evt.setScreenY(y);
	s_evt.setLeftMouseButton(true);
	s_evt.setEventCode(DfxSceneEvent::MOUSE_MOVE);
	s_evt.setScreenRay(calcScreenRay(x, y, width, height));
	s_evt.setView(currentView);

	if (currentView == DfxViewportView::PERSPECTIVE) {
		s_evt.setProjectionType(DfxProjectionType::PERSPECTIVE);
	}
	else {
		s_evt.setProjectionType(DfxProjectionType::ORTHOGRAPHIC);
	}

	if (viewState == DfxViewportState::GIZMO_SELECTED) {
		glm::vec3 handlepos;
		if (currentView != DfxViewportView::PERSPECTIVE) {
			DfxAbstractGizmo *gizmo = DfxScene::getInstance()->getActiveGizmo();
			GLubyte pixel[4];
			GLfloat dz = 0.f;

			makeCurrent();
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			cameraTransform();

			gizmo->draw(DfxAbstractGizmo::SELECT_MODE);
			glDisable(GL_DEPTH_TEST);
			glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);

			handlepos = glm::unProject(glm::vec3(x, height - y, dz),
				viewMat, orthoMat, glm::vec4(0, 0, width, height));

			s_evt.setScenePos(handlepos);
		}

		DfxScene::getInstance()->newSceneEvent(s_evt);
	}


	std::pair<int,int> seq(keyevent_key, mouseevent_button);


	if ( viewState == DfxViewportState::NO_STATE     ||
		 viewState == DfxViewportState::SCENE_PAN    ||
		 viewState == DfxViewportState::SCENE_ROTATE ||
		 viewState == DfxViewportState::SCENE_ZOOM ) {
		
		if (DfxShortcutManager::getInstance()->getViewportZoomSequence() == seq) {
			zoomViewPort(y, lastMousePosition.y);
			viewState = DfxViewportState::SCENE_ZOOM;
		}


		if (DfxShortcutManager::getInstance()->getViewportPanSequence() == seq) {
			panViewPort(x, y, lastMousePosition.x, lastMousePosition.y);
			viewState = DfxViewportState::SCENE_PAN;
		}


		if (DfxShortcutManager::getInstance()->getViewportRotateSequence() == seq) {
			rotateViewPort(x, y, lastMousePosition.x, lastMousePosition.y);
			viewState = DfxViewportState::SCENE_ROTATE;
		}
		

		lastMousePosition = glm::ivec2(x, y);
	}

	update();
}

void DfxViewportVersion01::keyPressEvent(QKeyEvent *evt) {
	keyLog.addKey(evt->key());
}

void DfxViewportVersion01::keyReleaseEvent(QKeyEvent *evt) {
	keyLog.removeKey(evt->key());
}


void DfxViewportVersion01::setBgColor(glm::vec3 top, glm::vec3 bottom) {
	bgColorTop = top;
	bgColorBottom = bottom;
}

DfxRay DfxViewportVersion01::calcScreenRay(int x, int y, int width, int height) {

	glm::vec3 start;
	glm::vec3 end;

	if (currentView == DfxViewportView::PERSPECTIVE) {
		start = glm::unProject(glm::vec3(x, height - y, 0),
			viewMat, perspectiveMat, glm::vec4(0, 0, width, height));

		end = glm::unProject(glm::vec3(x, height - y, 1),
			viewMat, perspectiveMat, glm::vec4(0, 0, width, height));
	}
	else {
		start = glm::unProject(glm::vec3(x, height - y, 0),
			viewMat, orthoMat, glm::vec4(0, 0, width, height));

		end = glm::unProject(glm::vec3(x, height - y, 1),
			viewMat, orthoMat, glm::vec4(0, 0, width, height));
	}

	DfxRay ray;

	ray.setNull(false);
	ray.start = start;
	ray.end = end;
	ray.origin = cameraPosition;
	ray.direction = glm::normalize(end - start);

	return ray;
}

void DfxViewportVersion01::switchToSolidRenderMode(bool st) {
	renderMode = DfxViewportRenderMode::SOLID;
	update();
}

void DfxViewportVersion01::switchToSolidWireRenderMode(bool st) {
	renderMode = DfxViewportRenderMode::SOLID_WIRE;
	update();
}

void DfxViewportVersion01::switchToWireRenderMode(bool st) {
	renderMode = DfxViewportRenderMode::WIRE;
	update();
}

void DfxViewportVersion01::switchShowHideGrid(bool st) {

	if (gridVisible)
		gridVisible = false;
	else
		gridVisible = true;

	update();
}

void DfxViewportVersion01::switchShowHideBackface(bool st) {

	if (backfaceVisible) {

		backfaceVisible = false;
		glDisable(GL_CULL_FACE);
	}
	else {

		backfaceVisible = true;
		glEnable(GL_CULL_FACE);
	}

	update();
}

void DfxViewportVersion01::enterEvent(QEvent *event) {

	if (!hasFocus()) {
		setFocus();
	}
}

void DfxViewportVersion01::renderSolidRenderMode() {
	switchToSolidRenderMode(true);
}

void DfxViewportVersion01::renderSolidWireRenderMode() {
	
}

void DfxViewportVersion01::renderWireRenderMode() {
	switchToWireRenderMode(true);
}

void DfxViewportVersion01::perspectiveView(bool st) {
	currentView = DfxViewportView::PERSPECTIVE;
	menus->setCurrentViewText("Perspective");
	update();
}

void DfxViewportVersion01::topView(bool st) {
	currentView = DfxViewportView::TOP;
	menus->setCurrentViewText("Top");
	update();
}

void DfxViewportVersion01::bottomView(bool st) {
	currentView = DfxViewportView::BOTTOM;
	menus->setCurrentViewText("Bottom");
	update();
}

void DfxViewportVersion01::leftView(bool st) {
	currentView = DfxViewportView::LEFT;
	menus->setCurrentViewText("Left");
	update();
}

void DfxViewportVersion01::rightView(bool st) {
	currentView = DfxViewportView::RIGHT;
	menus->setCurrentViewText("Right");
	update();
}

void DfxViewportVersion01::frontView(bool st) {
	currentView = DfxViewportView::FRONT;
	menus->setCurrentViewText("Front");
	update();
}

void DfxViewportVersion01::backView(bool st) {
	currentView = DfxViewportView::BACK;
	menus->setCurrentViewText("Back");
	update();
}

void DfxViewportVersion01::cameraFromView(bool st) {
	DfxGlobal::console::println("Camera From View");
}

void DfxViewportVersion01::cameraMenu() {
	
	std::vector<QAction*> oldActions;

	for (auto act : menus->camera->actions()) {
		oldActions.push_back(act);
	}

	for (auto act : oldActions) {
		menus->camera->removeAction(act);
		delete act;
	}

	for (auto cam : DfxScenePropRegistry::getIntstance()->findAllTypes(DfxScenePropRegistry::CAMERA)) {
		
		auto cam_t = (DfxCamera*)cam;
		QAction *act = new QAction(QString(cam_t->getLogicalName().c_str()));

		std::string label = cam_t->getLogicalName();

		glm::mat4 mat = cam_t->prefferedTransform();
		size_t iid = cam_t->getInstanceID();
		/*glm::vec3 pos;
		glm::quat rotQuat;
		glm::vec3 scl;
		glm::vec3 skew;
		glm::vec4 persp;

		glm::decompose(mat, scl, rotQuat, pos, skew, persp);

		rotQuat = glm::conjugate(rotQuat);
		Quat q;
		q.w = rotQuat.w;
		q.x = rotQuat.x;
		q.y = rotQuat.y;
		q.z = rotQuat.z;
		
		EulerAngles angles = { 0,0,0,EulOrdXYZr };
		angles = Eul_FromQuat(q, EulOrdXYZr);

		glm::vec3 rot;
		
		rot.x = angles.x;
		rot.y = angles.y;
		rot.z = angles.z;*/

		QObject::connect(act, &QAction::triggered, [this, iid, mat, label]() {
			
			cameraViewInfo.transform = glm::inverse(mat);
			cameraViewInfo.transform = mat;

			cameraViewInfo.cameraID = iid;

			menus->setCurrentViewText(label.c_str());
			currentView = DfxViewportView::CAMERA;

			update();
		});

		menus->camera->addAction(act);
	}
}