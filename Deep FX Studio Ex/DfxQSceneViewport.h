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

#include <QOpenglWidget>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <QMouseEvent>
#include <DfxColorCounter.h>
#include <DfxColorID.h>
#include <SgRootNode.h>
#include <DfxNullNode.h>
#include <DfxSceneSelectionModel.h>
#include <DfxColorField.h>
#include <QPainter>
#include <QSplitter>
#include "DfxCommon.h"
#include "DfxWorkGrid.h"
#include "DfxMathUtil.h"
#include "DfxWidgetInterface.h"
#include "DfxRay.h"
#include "DfxQViewportState.h"
#include "DfxQViewportRenderInterface.h"
#include "DfxWidgetManager.h"
#include "DfxMode.h"
#include "DfxMeshSelectionModel.h"
#include "DfxSelectionMarquee.h"
#include "DfxDeepSimplexGeoAdapter.h"
#include "DfxViewportOverlay.h"
#include "DfxLxCameraHandler.h"
#include "DfxSpace.h"
#include "DfxUpdater.h"
#include "DfxQSceneViewMenu.h"
#include "DfxSetTools.h"
#include "DfxViewportCommon.h"
#include "DfxQInfomationOverlay.h"
#include "DfxTransformHistoryAction.h"
#include "DfxHistoryActionArray.h"
#include "DfxHistoryManager.h"

class DfxQSceneViewport: public QOpenGLWidget {
	Q_OBJECT
private:
	class TimeObserver : public fl::Observer {
	public:
		DfxQSceneViewport *ptr;
		TimeObserver();
		void update(fl::Any *any) override;
	};

private:

	static size_t ID_GEN;
	size_t _id;

	DfxQViewportRenderInterface *renderer;

	DfxQViewportState stPerspective;
	DfxQViewportState stTop;
	DfxQViewportState stBottom;
	DfxQViewportState stLeft;
	DfxQViewportState stRight;
	DfxQViewportState stFront;
	DfxQViewportState stBack;
	DfxQViewportState stCamera;

	std::string cameraReferencePath;
	deepsg::SgNode *cameraNode;

	glm::mat4 orthoMat;
	glm::mat4 perspectiveMat;
	glm::mat4 viewMat;

	DfxViewportOverlay *overlay;
	DfxQInfomationOverlay *frameOverlay;

	TimeObserver *_timeObserverCurrentFrame;

	QLabel *currentFrame;

	DfxSelectionMarquee *selectionMarquee;

	std::vector<DfxTransformHistoryAction*> transformHistoryArray;

private:

	enum class Modes { NAVIGATION_SELECTION_MANIPULATION };
	enum class NSMModes { DEFUALT, GIZMO, NAVIGATION };
	enum class ViewState { PERSPECTIVE, TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK, CAMERA};

private:

	Modes mode;
	NSMModes nsmMode;
	ViewState viewstate;
	bool contextMenuActive;

	bool mouseButtonPressed;
	int mouseButton;
	glm::ivec2 mouseOnPressPosition;
	glm::ivec2 mouseOnReleasePosition;
	glm::ivec2 mouseOnMovePosition;
	glm::ivec2 mouseLastPosition;

	glm::vec3 eventCameraPosition;
	DfxWidgetInterface::OnMoveInfo widgetOnMoveInfo;
	DfxRay eventCameraRay;

private:

	void onMousePressInfo(QMouseEvent *event);
	void onMouseReleaseinfo(QMouseEvent *event);
	void onMouseMovePreinfo(QMouseEvent *event);
	void onMouseMovePostinfo(QMouseEvent *event);

	void NSMMode_onMousePress();
	void NSMMode_onMouseRelease ();
	void NSMMode_onMouseMove();

	void gizmoMode();
	void gizmoOnMoveMode();
	void navigationMode();

	void selectionMode();
	void objectSelectionMode();
	void shapeSelectionMode();
	void vertexSelectionMode();
	void edgeSelectionMode();
	void faceSelectionMode();

	bool isPan();
	bool isZoom();
	bool isRotate();

	void pan();
	void zoom();
	void rotate();
	
	void clearSelectedWidget();
	std::pair<deepsg::SgNode*, int> selectedWidget;
	std::pair<deepsg::SgNode*, int> isWidgetSelected();

	std::vector<DfxColorField> getMarqueeColors();

	glm::vec3 cameraPosition();

	void positionFrameInfomationOverlay();
	void updateFrameInfomation(QString info);

protected:

	//bool eventFilter(QObject *obj, QEvent *event) override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void initializeGL() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void resizeEvent(QResizeEvent *event) override;

	void drawSelectionMarquee();
	void setViewLabel();
public:

	DfxQSceneViewport(DfxQViewportRenderInterface *rnd, QWidget *parent = Q_NULLPTR);
	virtual ~DfxQSceneViewport();

	size_t id();

	DfxQViewportRenderInterface *getRenderer();

	void applyProjectionViewMatrix();

	void updateView();

	static void setPerspectiveViewState(DfxQViewportState &st);
	static void setTopViewState(DfxQViewportState &st);
	static void setBottomViewState(DfxQViewportState &st);
	static void setLeftViewState(DfxQViewportState &st);
	static void setRightViewState(DfxQViewportState &st);
	static void setFrontViewState(DfxQViewportState &st);
	static void setBackViewState(DfxQViewportState &st);

	void initOpenGlContext();
	DfxViewportOverlay *getOverlay();

public slots:

	void switchToPerspective();
	void switchToTop();
	void switchToBottom();
	void switchToFront();
	void switchToBack();
	void switchToLeft();
	void switchToRight();

	void switchToWireframeMode();
	void switchToWireframeSolidMode();
	void switchToSolidMode();

	void populateCameraMenu();
	void populateCameraFromViewMenu();

	void beginTransformHistory();
	void endTransformHistory();
	void clearTransformHistory();

	void resetViews();
};