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
#pragma once

#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QOpenGLFunctions_2_1> 
#include <QOpenGLFrameBufferObject>
#include <QOpenGLpaintDevice>
#include <QPainter>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm\geometric.hpp>
#include <glm\vec3.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\transform.hpp>
#include <QtWidgets\QPushButton>
#include <QtWidgets\QMenu>
#include <QtWidgets\QAction>
#include "DfxGlobal.h"
#include "DfxAbstractViewPort.h"
#include "DfxScene.h"
#include "DfxKeyLog.h"
#include "DfxRay.h"
#include "DfxViewportMenus.h"



enum class DfxViewportState { SCENE_ROTATE, SCENE_PAN, SCENE_ZOOM, NO_STATE, GIZMO_SELECTED };

enum class DfxViewportRenderMode { SOLID, SOLID_WIRE, WIRE };

typedef struct st_ViewInformation { 
	float viewPhi;
	float viewTheta;
	float viewLamda;
	float viewDepth;
	float viewPanX;
	float viewPanY;
	size_t cameraID;
	glm::mat4 transform;
}ViewInformation;

class DfxViewportVersion01: public DfxAbstractViewport, protected QOpenGLFunctions_2_1 {
	Q_OBJECT
public:
	DfxViewportVersion01();
	DfxViewportVersion01(QSurfaceFormat &);
	virtual ~DfxViewportVersion01();

protected:
	int keyevent_key;
	int mouseevent_button;
protected:
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void initializeGL() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	
	void enterEvent(QEvent *event) override;

	bool eventFilter(QObject *watched, QEvent *event) override;
private:

	bool gridVisible;

	bool backfaceVisible;

	DfxViewportView currentView;

	DfxViewportRenderMode renderMode;

	DfxViewportMenus *menus;

	void rotateViewPort(int currentX, int currentY, int lastX, int lastY);
	void panViewPort(int currentX, int currentY, int lastX, int lastY);
	void zoomViewPort(int currentY, int lastY);

	void cameraTransform();

	void drawObject(DfxObjectItem* ob);

	void drawScene();

	void drawSceneSelectionMode();

	void drawObjectSelectionMode(DfxObjectItem* ob);

	void drawGizmos();

	void drawGrid();

	void drawBackground();

	void preprocess();

	void reset();

	glm::ivec2 lastMousePosition;

	bool leftMouseBtnPressed;
	bool middleMouseBtnPressed;
	bool rightMouseBtnPressed;

	DfxViewportState viewState;

	ViewInformation perspViewInfo;
	ViewInformation topViewInfo;
	ViewInformation bottomViewInfo;
	ViewInformation leftViewInfo;
	ViewInformation rightViewInfo;
	ViewInformation frontViewInfo;
	ViewInformation backViewInfo;
	ViewInformation cameraViewInfo;

	float gizmoRatio;

	glm::mat4 orthoMat;
	glm::mat4 perspectiveMat;
	glm::mat4 viewMat;

	glm::vec3 cameraPosition;
	glm::vec3 orthoCameraPosition;

	uint gridHalfSize;
	float gridUnit;

	glm::vec3 bgColorTop;
	glm::vec3 bgColorBottom;

	DfxKeyLog keyLog;

	DfxRay calcScreenRay(int x, int y, int width, int height);
public:

	void setBgColor(glm::vec3 top, glm::vec3 bottom);

	QImage *capture() override;

public:

	void renderSolidRenderMode() override;
	void renderSolidWireRenderMode() override;
	void renderWireRenderMode() override;

public slots:

	// View Menu
	void perspectiveView(bool st);
	void topView(bool st);
	void bottomView(bool st);
	void leftView(bool st);
	void rightView(bool st);
	void frontView(bool st);
	void backView(bool st);
	void cameraFromView(bool st);
	void cameraMenu();

	// Render Mode Slots
	void switchToSolidRenderMode(bool st);
	void switchToSolidWireRenderMode(bool st);
	void switchToWireRenderMode(bool st);

	// Display Menu splots
	void switchShowHideGrid(bool st);
	void switchShowHideBackface(bool st);
};