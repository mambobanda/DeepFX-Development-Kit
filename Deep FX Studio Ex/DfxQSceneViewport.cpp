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

#include "DfxQSceneViewport.h"
#include "DfxViewportSynchronizer.h"
#include "DfxOpenGLCacheQueue.h"
#include "DfxSceneCleaner.h"
#include "DfxQMainWindow.h"

size_t DfxQSceneViewport::ID_GEN = 0xff;

DfxQSceneViewport::DfxQSceneViewport(DfxQViewportRenderInterface *rnd, QWidget *parent): QOpenGLWidget(parent) {
	_id = ID_GEN++;

	DfxViewportSynchronizer::addViewport(this);

	renderer = rnd;

	resetViews();

	mode = Modes::NAVIGATION_SELECTION_MANIPULATION;
	nsmMode = NSMModes::DEFUALT;
	viewstate = ViewState::PERSPECTIVE;

	selectedWidget = std::pair<deepsg::SgNode*, int>(nullptr, 0);

	overlay = new DfxViewportOverlay(this);
	frameOverlay = new DfxQInfomationOverlay(DfxQInfomationOverlay::BOTTOM_RIGHT, this);
	frameOverlay->layout()->setSizeConstraint(QLayout::SetMinimumSize);

	selectionMarquee = new DfxSelectionMarquee;
	renderer->setSelectionMarquee(selectionMarquee);

	currentFrame = new QLabel("Frame 0");
	currentFrame->setStyleSheet("QWidget{background-color: rgba(100,100,100,50);}");
	currentFrame->setMargin(3);

	cameraReferencePath = "";
	cameraNode = nullptr;

	contextMenuActive = false;

	frameOverlay->layout()->addWidget(currentFrame);
	positionFrameInfomationOverlay();

	_timeObserverCurrentFrame = new TimeObserver;

	_timeObserverCurrentFrame->ptr = this;

	DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame")->addObserver(_timeObserverCurrentFrame);
	
	QObject::connect(overlay->translateBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("translateTool"); });
	QObject::connect(overlay->rotateBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("rotateTool"); });
	QObject::connect(overlay->scaleBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("scaleTool"); });
	QObject::connect(overlay->objectModeBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("objectMode"); });
	QObject::connect(overlay->vertexModeBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("vertexMode"); });
	QObject::connect(overlay->edgeModeBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("edgeMode"); });
	QObject::connect(overlay->faceModeBtn, &QPushButton::clicked, []() { DfxGlobal::command::exec("faceMode"); });

	QObject::connect(overlay->perspective, &QAction::triggered, this, &DfxQSceneViewport::switchToPerspective);
	QObject::connect(overlay->top, &QAction::triggered, this, &DfxQSceneViewport::switchToTop);
	QObject::connect(overlay->bottom, &QAction::triggered, this, &DfxQSceneViewport::switchToBottom);
	QObject::connect(overlay->back, &QAction::triggered, this, &DfxQSceneViewport::switchToBack);
	QObject::connect(overlay->front, &QAction::triggered, this, &DfxQSceneViewport::switchToFront);
	QObject::connect(overlay->left, &QAction::triggered, this, &DfxQSceneViewport::switchToLeft);
	QObject::connect(overlay->right, &QAction::triggered, this, &DfxQSceneViewport::switchToRight);

	QObject::connect(overlay->wireframe, &QAction::triggered, this, &DfxQSceneViewport::switchToWireframeMode);
	QObject::connect(overlay->wireSolidShade, &QAction::triggered, this, &DfxQSceneViewport::switchToWireframeSolidMode);
	QObject::connect(overlay->solidShade, &QAction::triggered, this, &DfxQSceneViewport::switchToSolidMode);

	QObject::connect(overlay->viewModeMenu, &QMenu::aboutToShow, this, &DfxQSceneViewport::populateCameraMenu);
	QObject::connect(overlay->viewModeMenu, &QMenu::aboutToShow,this, &DfxQSceneViewport::populateCameraFromViewMenu);
	
	QObject::connect(DfxSceneCleaner::getInstance(), &DfxSceneCleaner::resetSignal, this, &DfxQSceneViewport::resetViews);

}

DfxQSceneViewport::~DfxQSceneViewport() {
	DfxViewportSynchronizer::removeViewport(this);
	delete selectionMarquee;
	delete _timeObserverCurrentFrame;
}

DfxQViewportRenderInterface *DfxQSceneViewport::getRenderer() {
	return renderer;
}

void DfxQSceneViewport::resizeGL(int w, int h) {
	renderer->resize(w,h);
}

size_t DfxQSceneViewport::id() {
	return _id;
}

void DfxQSceneViewport::updateView() {
	update();
	if (isHidden() || !isVisible()) {
		if (!DfxViewportSynchronizer::synchronized()) {
			DfxViewportSynchronizer::decrementCounter();
		}
	}
}

void DfxQSceneViewport::paintGL() {
	makeCurrent();

	DfxOpenGLCacheQueue::getInstance()->cacheAndClear();

	applyProjectionViewMatrix();
	renderer->setCameraPosition(cameraPosition());
	renderer->setCameraPath(cameraReferencePath);
	if(cameraNode!=nullptr)renderer->setCameraTransform(deepsg::SgRootNode::getInstance()->getTransformation4x4(cameraNode));
	else renderer->setCameraTransform(glm::mat4(1.f));
	renderer->setWorkGrid(*DfxWorkGrid::getInstance());
	renderer->setBackgroundColor(glm::vec3(0.2f, 0.2f, 0.2f));
	renderer->render();
	renderer->drawCameraRenderRegion(size().width(), size().height());

	if (!DfxViewportSynchronizer::synchronized()) DfxViewportSynchronizer::decrementCounter();

	drawSelectionMarquee();
	setViewLabel();
}

DfxViewportOverlay *DfxQSceneViewport::getOverlay() {
	return overlay;
}

void DfxQSceneViewport::initializeGL() {
	context()->setShareContext(QOpenGLContext::globalShareContext());
	renderer->init();
}

void DfxQSceneViewport::onMousePressInfo(QMouseEvent *event) {
	mouseButtonPressed = true;
	mouseButton = event->button();
	mouseOnPressPosition = glm::ivec2(event->x(), event->y());
	mouseLastPosition = mouseOnPressPosition;
}

void DfxQSceneViewport::onMouseReleaseinfo(QMouseEvent *event) {
	mouseButtonPressed = false;
	mouseButton = event->button();
	mouseOnReleasePosition = glm::ivec2(event->x(), event->y());
}

void DfxQSceneViewport::onMouseMovePreinfo(QMouseEvent *event) {
	mouseOnMovePosition = glm::ivec2(event->x(), event->y());
}

void DfxQSceneViewport::onMouseMovePostinfo(QMouseEvent *event) {
	mouseLastPosition = glm::ivec2(event->x(), event->y());
}

void DfxQSceneViewport::mousePressEvent(QMouseEvent *event) {

	onMousePressInfo(event);

	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (kbuffer->keys().size() == 0 && event->button() == Qt::RightButton) {
			DfxQSceneViewMenu::exec();
			contextMenuActive = true;
		}
		else {
			if (mode == Modes::NAVIGATION_SELECTION_MANIPULATION) {
				NSMMode_onMousePress();
			}
		}
	}
}

void DfxQSceneViewport::mouseReleaseEvent(QMouseEvent *event) {

	onMouseReleaseinfo(event);

	if (contextMenuActive) {
		contextMenuActive = false;
	}
	else {
		if (mode == Modes::NAVIGATION_SELECTION_MANIPULATION) {
			NSMMode_onMouseRelease();
		}
	}
	
}

void DfxQSceneViewport::mouseMoveEvent(QMouseEvent *event) {

	onMouseMovePreinfo(event);

	if (mode == Modes::NAVIGATION_SELECTION_MANIPULATION &&
		mouseButtonPressed) {

		NSMMode_onMouseMove();
	}

	onMouseMovePostinfo(event);
	
}

void DfxQSceneViewport::NSMMode_onMousePress() {
	gizmoMode();
}

void DfxQSceneViewport::NSMMode_onMouseRelease() {

	if (nsmMode == NSMModes::DEFUALT) {
		selectionMarquee->setEndX(mouseOnReleasePosition.x);
		selectionMarquee->setEndY(mouseOnReleasePosition.y);
		selectionMode();
	}

	if (nsmMode == NSMModes::GIZMO) {
		endTransformHistory();
		DfxUpdater::getInstance()->nodeTransformUpdated(DfxSceneSelectionModel::getInstance()->selected());
	}

	clearSelectedWidget();
	selectionMarquee->setIsActive(false);
	nsmMode = NSMModes::DEFUALT;
}

void DfxQSceneViewport::NSMMode_onMouseMove() {

	if (nsmMode == NSMModes::GIZMO) {
		gizmoOnMoveMode();
	}
	else if (nsmMode == NSMModes::DEFUALT || nsmMode == NSMModes::NAVIGATION) {
		navigationMode();
	}
}

void DfxQSceneViewport::gizmoMode() {

	selectedWidget = isWidgetSelected();	
	if (selectedWidget.first != nullptr) {
		beginTransformHistory();

		int _w = size().width(); int _h = size().height();
		eventCameraPosition = cameraPosition();
		eventCameraRay = math::calcScreenRay(mouseOnPressPosition.x, mouseOnPressPosition.y, _w, _h, viewMat, perspectiveMat, eventCameraPosition);
		widgetOnMoveInfo.previous = eventCameraRay;
		widgetOnMoveInfo.screenPosX_previous = mouseOnPressPosition.x;
		widgetOnMoveInfo.screenPosY_previous = mouseOnPressPosition.y;

		DfxWidgetInterface::OnClickInfo onClickInfo;
		onClickInfo.current = eventCameraRay;
		onClickInfo.screenPosX = mouseOnPressPosition.x;
		onClickInfo.screenPosY = mouseOnPressPosition.y;

		auto width = (DfxWidgetInterface*)((deepsg::SgWidgetNode*)selectedWidget.first)->widget();
		width->onClick(selectedWidget.second, onClickInfo);

		nsmMode = NSMModes::GIZMO;
	}
}

void DfxQSceneViewport::gizmoOnMoveMode() {

	if (selectedWidget.first != nullptr){
		int _w = size().width(); int _h = size().height();

		if (viewstate != ViewState::PERSPECTIVE) {
			widgetOnMoveInfo.current = math::calcScreenRay(mouseOnMovePosition.x, mouseOnMovePosition.y, _w, _h, viewMat, perspectiveMat, eventCameraPosition);

			widgetOnMoveInfo.type = DfxWidgetInterface::OnMoveInfo::SCREEN;
			
			if (viewstate == ViewState::BACK) widgetOnMoveInfo.view		= DfxWidgetInterface::OnMoveInfo::BACK;
			if (viewstate == ViewState::FRONT) widgetOnMoveInfo.view	= DfxWidgetInterface::OnMoveInfo::FRONT;
			if (viewstate == ViewState::RIGHT) widgetOnMoveInfo.view	= DfxWidgetInterface::OnMoveInfo::RIGHT;
			if (viewstate == ViewState::LEFT) widgetOnMoveInfo.view		= DfxWidgetInterface::OnMoveInfo::LEFT;
			if (viewstate == ViewState::TOP) widgetOnMoveInfo.view		= DfxWidgetInterface::OnMoveInfo::TOP;
			if (viewstate == ViewState::BOTTOM) widgetOnMoveInfo.view	= DfxWidgetInterface::OnMoveInfo::BOTTOM;
		}
		else {
			widgetOnMoveInfo.current = math::calcScreenRay(mouseOnMovePosition.x, mouseOnMovePosition.y, _w, _h, viewMat, perspectiveMat, eventCameraPosition);
			widgetOnMoveInfo.type = DfxWidgetInterface::OnMoveInfo::RAY;
		}

		widgetOnMoveInfo.screenPosX_current = mouseOnMovePosition.x;
		widgetOnMoveInfo.screenPosY_current = mouseOnMovePosition.y;

		auto widt = (DfxWidgetInterface*)((deepsg::SgWidgetNode*)selectedWidget.first)->widget();
		widt->onMove(selectedWidget.second, widgetOnMoveInfo);

		widgetOnMoveInfo.previous = widgetOnMoveInfo.current;
		widgetOnMoveInfo.screenPosX_previous = mouseOnMovePosition.x;
		widgetOnMoveInfo.screenPosY_previous = mouseOnMovePosition.y;

		DfxUpdater::getInstance()->sceneUpdated();
	}
}

void DfxQSceneViewport::navigationMode() {

	if (nsmMode == NSMModes::DEFUALT || nsmMode == NSMModes::NAVIGATION) {
		if (isPan()) {
			nsmMode = NSMModes::NAVIGATION;
			pan();
		}
		else if (isZoom()) {
			nsmMode = NSMModes::NAVIGATION;
			zoom();
		}
		else if (isRotate()) {
			nsmMode = NSMModes::NAVIGATION;
			rotate();
		}
		else {

			selectionMarquee->setIsActive(true);
			selectionMarquee->setStartX(mouseOnPressPosition.x);
			selectionMarquee->setStartY(mouseOnPressPosition.y);
			selectionMarquee->setEndX(mouseOnMovePosition.x);
			selectionMarquee->setEndY(mouseOnMovePosition.y);
			selectionMarquee->setCanvasWidth(size().width());
			selectionMarquee->setCanvasHeight(size().height());
		}
	}

	DfxUpdater::getInstance()->sceneUpdated();
}

void DfxQSceneViewport::beginTransformHistory() {
	clearTransformHistory();

	for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
		if (node->xFormProperties() != nullptr) {
			auto historyAction = new DfxTransformHistoryAction;
			historyAction->setReference(node->xFormProperties()->absolutePath());
			historyAction->setOldTransform(node->xFormProperties());
			transformHistoryArray.push_back(historyAction);
		}
	}
}

void DfxQSceneViewport::endTransformHistory() {
	int counter = 0;

	for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
		if (node->xFormProperties() != nullptr) {
			transformHistoryArray[counter]->setNewTransform(node->xFormProperties());
			transformHistoryArray[counter]->buildHistoryActions();
			counter++;
		}
	}

	if (!transformHistoryArray.empty()) {
		auto historyArray = new DfxHistoryActionArray;
		
		for (auto act: transformHistoryArray) {
			historyArray->addAction(act);
		}

		DfxHistoryManager::getInstance()->pushHistoryAction(historyArray);

		transformHistoryArray.clear();
	}
}

void DfxQSceneViewport::clearTransformHistory() {

	for (auto item : transformHistoryArray) {
		delete item;
	}

	transformHistoryArray.clear();
}

void DfxQSceneViewport::objectSelectionMode() {

	DfxColorCounter ccounter;
	glm::ivec4 selectedColor(0);
	std::vector<DfxColorField> selectedColors;
	bool clearSelection = true;
	bool append = false;

	// Give elements unique color
	deepsg::SgConditionalTraversalFunction colorObjectsFunc = [&ccounter](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {

		if (node->isVisible()) {
			if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
				auto geonode = (deepsg::SgGeoNode*)node;
				geonode->setColor(glm::ivec3(ccounter.redi(), ccounter.greeni(), ccounter.bluei()));
				ccounter.increment();
			}
			else if (node->getType() == deepsg::SgNodeType::SHAPE) {
				auto shapenode = (deepsg::SgShapeNode*)node;
				shapenode->setColor(glm::ivec3(ccounter.redi(), ccounter.greeni(), ccounter.bluei()));
				ccounter.increment();
			}
			else if (node->getType() == deepsg::SgNodeType::NILL) {
				auto null_node = (DfxNullNode*)node;
				null_node->setColor(glm::ivec3(ccounter.redi(), ccounter.greeni(), ccounter.bluei()));
				ccounter.increment();
			}
			else if (node->getType() == deepsg::SgNodeType::STAGE_PROP) {
				auto prop_node = (deepsg::SgStagePropNode*)node;
				prop_node->setColor(glm::ivec3(ccounter.redi(), ccounter.greeni(), ccounter.bluei()));
				ccounter.increment();
			}

			return deepsg::TraversalFlag::CONTINUE;
		}
		else {
			return deepsg::TraversalFlag::IGNORE_CHILDREN;
		}
	};

	// Add or Remove from selection list
	deepsg::SgConditionalTraversalFunction selectObjectFunc = [this, &append, &clearSelection, &selectedColor, &selectedColors](deepsg::SgNode *node, deepsg::SgStateInfo *info)
		-> deepsg::TraversalFlag {
		
		if (node->isVisible()) {
			bool process = false;
			glm::ivec3 _color(0);

			if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
				_color = ((deepsg::SgGeoNode*)node)->getColor();
				process = true;
			}
			else if (node->getType() == deepsg::SgNodeType::SHAPE) {
				_color = ((deepsg::SgShapeNode*)node)->getColor();
				process = true;
			}
			else if (node->getType() == deepsg::SgNodeType::NILL) {
				_color = ((DfxNullNode*)node)->getColor();
				process = true;
			}
			else if (node->getType() == deepsg::SgNodeType::STAGE_PROP) {
				_color = ((deepsg::SgStagePropNode*)node)->getColor();
				process = true;
			}

			if (process) {

				if (selectionMarquee->isActive()) {
					DfxColorField cid00(_color.x, _color.y, _color.z);

					for (auto cid : selectedColors) {
						if (cid.value() == cid00.value()) {

							DfxSceneSelectionModel::getInstance()->addRemoveSelection(node);

							clearSelection = false;
						}
					}
				}
				else {

					DfxColorID cid00(selectedColor.x, selectedColor.y, selectedColor.z);
					DfxColorID cid01(_color.x, _color.y, _color.z);

					if (cid00.integer_str() == cid01.integer_str()) {

						DfxSceneSelectionModel::getInstance()->addRemoveSelection(node);

						clearSelection = false;
					}
				}
			}

			return deepsg::TraversalFlag::CONTINUE;
		}
		else {
			return deepsg::TraversalFlag::IGNORE_CHILDREN;
		}
	};

	deepsg::SgRootNode::getInstance()->traverseFluentSceneConditionally(colorObjectsFunc);

	makeCurrent();
	applyProjectionViewMatrix();
	renderer->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f));
	renderer->renderColors(DfxQViewportRenderInterface::ElementType::OBJECT_SHAPE);

	if (selectionMarquee->isActive()) {
		selectedColors = getMarqueeColors();	
	}
	else {
		selectedColor = renderer->colorAtPosition(mouseOnReleasePosition.x, mouseOnReleasePosition.y, size().height());
	}

	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (kbuffer->keys().size() > 0) {
			if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {
				append = true;
				deepsg::SgRootNode::getInstance()->traverseFluentSceneConditionally(selectObjectFunc);
			}
		}
		else {
			if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
				DfxSceneSelectionModel::getInstance()->clearSelection();
				deepsg::SgRootNode::getInstance()->traverseFluentSceneConditionally(selectObjectFunc);
			}
		}
	}

	if (clearSelection) DfxSceneSelectionModel::getInstance()->clearSelection();

	DfxUpdater::getInstance()->sceneUpdated();
}


void DfxQSceneViewport::vertexSelectionMode() {
	bool clearSelection = true;

	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

		makeCurrent();
		applyProjectionViewMatrix();
		renderer->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f));
		renderer->renderColors(DfxQViewportRenderInterface::ElementType::VERTEX);

		if (selectionMarquee->isActive()) {
			std::vector<DfxColorField> selectedColors = getMarqueeColors();
			
			if (DfxQMainWindow::isReady()) {
				auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

				if (kbuffer->keys().size() > 0) {

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasVertexIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedVertexIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedVertexIndices().end(), cf.value() - 1);
								}
								else {
									if (cf.value() > 0) {
										DfxMeshSelectionModel::getInstance()->selectedVertexIndices().push_back(cf.value() - 1);
									}
								}
								clearSelection = false;
							}
						}
					}
				}
				else {
					DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedVertexIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}

			}
			
		}
		else {
			glm::ivec4 sc = renderer->colorAtPosition(mouseOnReleasePosition.x, mouseOnReleasePosition.y, size().height());
			DfxColorField cf(sc.x, sc.y, sc.z);

			if (cf.value() > 0) {
				if (DfxQMainWindow::isReady()) {
					auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

					if (kbuffer->keys().size() > 0) {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasVertexIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedVertexIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedVertexIndices().end(), cf.value() - 1);
								}
								else {
									DfxMeshSelectionModel::getInstance()->selectedVertexIndices().push_back(cf.value() - 1);
								}
								clearSelection = false;
							}
						}
					}
					else {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
							DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedVertexIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}
			}
		}

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();

			DfxMeshSelectionModel::getInstance()->calcVertexCenter(mesh);
		}
	}

	if (clearSelection) DfxMeshSelectionModel::getInstance()->selectedVertexIndices().clear();

	DfxUpdater::getInstance()->sceneUpdated();
}

void DfxQSceneViewport::edgeSelectionMode() {
	bool clearSelection = true;

	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

		makeCurrent();
		applyProjectionViewMatrix();
		renderer->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f));
		renderer->renderColors(DfxQViewportRenderInterface::ElementType::EDGE);

		if (selectionMarquee->isActive()) {
			std::vector<DfxColorField> selectedColors = getMarqueeColors();

			if (DfxQMainWindow::isReady()) {
				auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

				if (kbuffer->keys().size() > 0) {

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasEdgeIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().end(), cf.value() - 1);
								}
								else {
									if (cf.value() > 0) {
										DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(cf.value() - 1);
									}
								}
								clearSelection = false;
							}
						}
					}
				}
				else {
					DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}
			}
		}
		else {
			glm::ivec4 sc = renderer->colorAtPosition(mouseOnReleasePosition.x, mouseOnReleasePosition.y, size().height());
			DfxColorField cf(sc.x, sc.y, sc.z);

			if (cf.value() > 0) {

				if (DfxQMainWindow::isReady()) {
					auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

					if (kbuffer->keys().size() > 0) {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasEdgeIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().end(), cf.value() - 1);
								}
								else {
									DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(cf.value() - 1);
								}
								clearSelection = false;
							}
						}
					}
					else {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
							DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}
			}
		}

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();

			DfxMeshSelectionModel::getInstance()->calcEdgeCenter(mesh);
		}
	}

	if (clearSelection) DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().clear();

	DfxUpdater::getInstance()->sceneUpdated();
}

void DfxQSceneViewport::faceSelectionMode() {
	bool clearSelection = true;

	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

		makeCurrent();
		applyProjectionViewMatrix();
		renderer->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f));
		renderer->renderColors(DfxQViewportRenderInterface::ElementType::FACE);

		if (selectionMarquee->isActive()) {
			std::vector<DfxColorField> selectedColors = getMarqueeColors();

			if (DfxQMainWindow::isReady()) {
				auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

				if (kbuffer->keys().size() > 0) {

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasFaceIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedFaceIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedFaceIndices().end(), cf.value() - 1);
								}
								else {
									if (cf.value() > 0) {
										DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(cf.value() - 1);
									}
								}
								clearSelection = false;
							}
						}
					}
				}
				else {
					DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();

					if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
						for (auto cf : selectedColors) {
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}
			}
		}
		else {
			glm::ivec4 sc = renderer->colorAtPosition(mouseOnReleasePosition.x, mouseOnReleasePosition.y, size().height());
			DfxColorField cf(sc.x, sc.y, sc.z);

			if (cf.value() > 0) {

				if (DfxQMainWindow::isReady()) {
					auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

					if (kbuffer->keys().size() > 0) {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Multi Select", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton)) {

							if (cf.value() > 0) {
								if (DfxMeshSelectionModel::getInstance()->hasFaceIndex(cf.value() - 1)) {
									std::remove(DfxMeshSelectionModel::getInstance()->selectedFaceIndices().begin(),
										DfxMeshSelectionModel::getInstance()->selectedFaceIndices().end(), cf.value() - 1);
								}
								else {
									DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(cf.value() - 1);
								}
								clearSelection = false;
							}
						}
					}
					else {
						if (DfxShortcutsDB::getInstance()->match("Viewport", "Select", DfxShortcutsDB::KEYBOARD_MOUSE, 0, 0, 0, 0, mouseButton)) {
							DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
							if (cf.value() > 0) {
								DfxMeshSelectionModel::getInstance()->selectedFaceIndices().push_back(cf.value() - 1);
								clearSelection = false;
							}
						}
					}
				}
			}
		}

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();

			DfxMeshSelectionModel::getInstance()->calcFaceCenter(mesh);
		}
	}

	if (clearSelection) DfxMeshSelectionModel::getInstance()->selectedFaceIndices().clear();
	DfxMeshSelectionModel::getInstance()->generateSelectedFaceTriangleIndices();

	DfxUpdater::getInstance()->sceneUpdated();
}

void DfxQSceneViewport::selectionMode() {

	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
		objectSelectionMode();
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
		vertexSelectionMode();
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
		edgeSelectionMode();
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		faceSelectionMode();
	}
}

std::pair<deepsg::SgNode*, int> DfxQSceneViewport::isWidgetSelected() {
	
	std::pair<deepsg::SgNode*, int> rt(nullptr, 0);
	glm::ivec4 selectedColor(0);
	makeCurrent();

	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {
			
		DfxColorCounter ccounter;
		auto wnode = DfxWidgetManager::getInstance()->getWidget();
		std::vector<DfxColorID> ids;

		for (auto h : wnode->widget()->handles()) {
			ids.push_back(DfxColorID(ccounter.redi(), ccounter.greeni(), ccounter.bluei()));
			ccounter.increment();
		}
			
		renderer->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f));
		renderer->renderWidget(ids);
		glm::ivec4 sc = renderer->colorAtPosition(mouseOnPressPosition.x, mouseOnPressPosition.y, size().height());

		int counter = 0;
		for (auto id : ids) {
				
			if (id.integer_str() == DfxColorID(sc.x, sc.y, sc.z).integer_str()) {
				rt = std::pair<deepsg::SgNode*, int>(wnode, counter);
				auto widt = wnode->widget();
				break;
			}
					
			counter++;
		}
	}

	DfxUpdater::getInstance()->sceneUpdated();

	return rt;
}

bool DfxQSceneViewport::isPan() {
	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (kbuffer->keys().size() > 0) {
			return DfxShortcutsDB::getInstance()->match("Viewport", "Pan", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton);
		}
	}

	return false;
}

bool DfxQSceneViewport::isZoom() {
	
	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (kbuffer->keys().size() > 0) {
			return DfxShortcutsDB::getInstance()->match("Viewport", "Zoom", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton);
		}
	}

	return false;
}

bool DfxQSceneViewport::isRotate() {

	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (kbuffer->keys().size() > 0) {
			return DfxShortcutsDB::getInstance()->match("Viewport", "Rotate", DfxShortcutsDB::KEYBOARD_MOUSE, kbuffer->keys()[0], 0, 0, 0, mouseButton);
		}
	}

	return false;
}

void DfxQSceneViewport::pan() {

	if (viewstate == ViewState::PERSPECTIVE) {
		auto weight = std::abs(stPerspective.viewDepth / 8.0f);

		stPerspective.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stPerspective.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::TOP) {
		auto weight = std::abs(stTop.viewDepth / 8.0f);

		stTop.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stTop.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::BOTTOM) {
		auto weight = std::abs(stBottom.viewDepth / 8.0f);

		stBottom.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stBottom.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::LEFT) {
		auto weight = std::abs(stLeft.viewDepth / 8.0f);

		stLeft.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stLeft.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::RIGHT) {
		auto weight = std::abs(stRight.viewDepth / 8.0f);

		stRight.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stRight.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::FRONT) {
		auto weight = std::abs(stFront.viewDepth / 8.0f);

		stFront.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stFront.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}
	else if (viewstate == ViewState::BACK) {
		auto weight = std::abs(stBack.viewDepth / 8.0f);

		stBack.viewPanX += ((mouseOnMovePosition.x - mouseLastPosition.x)*0.01) * weight;
		stBack.viewPanY += ((mouseOnMovePosition.y - mouseLastPosition.y)*0.01) * weight;
	}

}

void DfxQSceneViewport::zoom() {

	float baseDepth = -8.0f;
	float newDepth = ((float)(mouseLastPosition.y - mouseOnMovePosition.y) / 10.0)* 0.2f;
	
	if (viewstate == ViewState::PERSPECTIVE) {
		float finalDepth = stPerspective.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stPerspective.viewDepth += viewDepth;
	}
	else if (viewstate == ViewState::TOP) {
		float finalDepth = stTop.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stTop.viewDepth -= viewDepth;
	}
	else if (viewstate == ViewState::BOTTOM) {
		float finalDepth = stBottom.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stBottom.viewDepth -= viewDepth;
	}
	else if (viewstate == ViewState::LEFT) {
		float finalDepth = stLeft.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stLeft.viewDepth -= viewDepth;
	}
	else if (viewstate == ViewState::RIGHT) {
		float finalDepth = stRight.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stRight.viewDepth -= viewDepth;
	}
	else if (viewstate == ViewState::FRONT) {
		float finalDepth = stFront.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stFront.viewDepth -= viewDepth;
	}
	else if (viewstate == ViewState::BACK) {
		float finalDepth = stBack.viewDepth + newDepth;
		float viewDepth = newDepth * (finalDepth / baseDepth);

		stBack.viewDepth -= viewDepth;
	}

}

void DfxQSceneViewport::rotate() {

	if (viewstate == ViewState::PERSPECTIVE) {
		stPerspective.viewPhi += (float)(mouseOnMovePosition.x - mouseLastPosition.x) / 4.0; stPerspective.viewTheta += (float)(mouseLastPosition.y - mouseOnMovePosition.y) / 4.0;
	}
}

void DfxQSceneViewport::clearSelectedWidget() {
	if(selectedWidget.first != nullptr) {
		auto widt = (DfxWidgetInterface*)((deepsg::SgWidgetNode*)selectedWidget.first)->widget();
		
		int _w = size().width(); int _h = size().height();
		auto _eventCameraPosition = cameraPosition();
		auto _eventCameraRay = math::calcScreenRay(mouseOnReleasePosition.x, mouseOnReleasePosition.y, _w, _h, viewMat, perspectiveMat, eventCameraPosition);

		DfxWidgetInterface::OnReleaseInfo onReleaseInfo;
		onReleaseInfo.current = _eventCameraRay;
		onReleaseInfo.screenPosX = mouseOnReleasePosition.x;
		onReleaseInfo.screenPosY = mouseOnReleasePosition.y;

		widt->onRelease(selectedWidget.second, onReleaseInfo);

		selectedWidget.first = nullptr;
		selectedWidget.second = 0;
	}
}

void DfxQSceneViewport::applyProjectionViewMatrix() {

	int w = size().width();
	int h = size().height();

	float aspectRatio = (GLfloat)w / (GLfloat)h;

	if (viewstate == ViewState::PERSPECTIVE) {
		perspectiveMat = glm::perspective(45.0f, (float)w / h, 0.05f, 500.0f);

		viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stPerspective.viewPanX, -stPerspective.viewPanY, stPerspective.viewDepth));

		viewMat *= glm::rotate(glm::radians(-stPerspective.viewTheta), glm::vec3(1.0f, 0.0f, 0.0f));

		viewMat *= glm::rotate(glm::radians(stPerspective.viewPhi), glm::vec3(0.0f, 1.0f, 0.0f));

		viewMat *= glm::rotate(glm::radians(stPerspective.viewLamda), glm::vec3(0.0f, 0.0f, 1.0f));

		renderer->setProjectionMatrix(perspectiveMat);
		renderer->setViewMatrix(viewMat);

	}else if (viewstate == ViewState::CAMERA) {
		auto any = DfxSpace::getInstance()->findAbsolute(cameraReferencePath);
		bool hasCameraInfo = false;

		if (any != nullptr) {
			fl::DataObject *dao = nullptr;

			if (any->type() == fl::type::DATA_OBJECT) {
				dao = any->rawData<fl::DataObject>();

				if (dao->hasProperty("width") && dao->hasProperty("height") && 
					dao->hasProperty("field_of_view") && dao->hasProperty("xform")) {

					float fov = dao->property("field_of_view")->data<float>();

					float cam_w = dao->property("width")->data<int>();
					float cam_h = dao->property("height")->data<int>();

					perspectiveMat = glm::perspective(glm::radians(fov), (float)w / (float)h, 0.05f, 500.0f);
					viewMat = glm::inverse(deepsg::SgRootNode::getInstance()->getTransformation4x4(cameraNode));

					renderer->setProjectionMatrix(perspectiveMat);
					renderer->setViewMatrix(viewMat);

					hasCameraInfo = true;
				}
			}
		}

		if (!hasCameraInfo) {
			DfxLxCameraHandler handler;
			handler.fetch(cameraReferencePath);

			float fov = 60.f;
			float cam_w = 500.f;
			float cam_h = 500.f;
			static float proj[16];

			auto snode = handler.shapeNode();

			perspectiveMat = glm::perspective(glm::radians(fov), (float)w / h, 0.05f, 500.0f);
			viewMat = glm::inverse(deepsg::SgRootNode::getInstance()->getTransformation4x4(snode));

			renderer->setProjectionMatrix(perspectiveMat);
			renderer->setViewMatrix(viewMat);
	
		}

	}
	else if(viewstate == ViewState::TOP || viewstate == ViewState::BOTTOM || viewstate == ViewState::BACK ||
		    viewstate == ViewState::FRONT || viewstate == ViewState::LEFT || viewstate == ViewState::RIGHT){
		
		DfxQViewportState currentViewinfo;

		if (viewstate == ViewState::TOP) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stTop.viewPanX, -stTop.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(1.0f, 0.0f, 0.0f));

			currentViewinfo = stTop;
		}
		else if (viewstate == ViewState::BOTTOM) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stBottom.viewPanX, -stBottom.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));

			currentViewinfo = stBottom;
		}
		else if (viewstate == ViewState::LEFT) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stLeft.viewPanX, -stLeft.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(-90.f), glm::vec3(0.0f, 1.0f, 0.0f));

			currentViewinfo = stLeft;
		}
		else if (viewstate == ViewState::RIGHT) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stRight.viewPanX, -stRight.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));

			currentViewinfo = stRight;
		}
		else if (viewstate == ViewState::FRONT) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stFront.viewPanX, -stFront.viewPanY, 0.0f));
			//viewMat *= glm::rotate(glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));

			currentViewinfo = stFront;
		}
		else if (viewstate == ViewState::BACK) {
			viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(stBack.viewPanX, -stBack.viewPanY, 0.0f));
			viewMat *= glm::rotate(glm::radians(180.f), glm::vec3(0.0f, 0.0f, 1.0f));
			viewMat *= glm::rotate(glm::radians(180.f), glm::vec3(1.0f, 0.0f, 0.0f));

			currentViewinfo = stBack;
		}

		if (w <= h) {
			orthoMat = glm::ortho(
				-currentViewinfo.viewDepth, currentViewinfo.viewDepth,
				-currentViewinfo.viewDepth / aspectRatio,
				currentViewinfo.viewDepth / aspectRatio,
				-100.f, 100.f);
		}
		else {
			orthoMat = glm::ortho(
				-currentViewinfo.viewDepth * aspectRatio, currentViewinfo.viewDepth * aspectRatio,
				-currentViewinfo.viewDepth, currentViewinfo.viewDepth,
				-100.1f, 100.f);
		}

		renderer->setProjectionMatrix(orthoMat);
		renderer->setViewMatrix(viewMat);
	}
}

glm::vec3 DfxQSceneViewport::cameraPosition(){
	glm::vec3 cameraPosition(0);

	if (viewstate == ViewState::PERSPECTIVE || viewstate == ViewState::CAMERA) {
		cameraPosition = math::extractCameraPosition(viewMat);
	}
	else {

		if (viewstate == ViewState::TOP) {
			cameraPosition = glm::vec3(stTop.viewDepth * 2.f, stTop.viewDepth * 2.f, 0.f);
		}
		else if (viewstate == ViewState::BOTTOM) {
			cameraPosition = glm::vec3(stBottom.viewDepth * 2.f, stBottom.viewDepth * 2.f, 0.f);
		}
		else if (viewstate == ViewState::LEFT) {
			cameraPosition = glm::vec3(stLeft.viewDepth * 2.f, stLeft.viewDepth * 2.f, 0.f);
		}
		else if (viewstate == ViewState::RIGHT) {
			cameraPosition = glm::vec3(stRight.viewDepth * 2.f, stRight.viewDepth * 2.f, 0.f);
		}
		else if (viewstate == ViewState::FRONT) {
			cameraPosition = glm::vec3(stFront.viewDepth * 2.f, stFront.viewDepth * 2.f, 0.f);
		}
		else if (viewstate == ViewState::BACK) {
			cameraPosition = glm::vec3(stBack.viewDepth * 2.f, stBack.viewDepth * 2.f, 0.f);
		}

	}

	return cameraPosition;
}

void DfxQSceneViewport::setPerspectiveViewState(DfxQViewportState &st) {

	st.viewPhi = -37.75;
	st.viewTheta = -27;
	st.viewLamda = 0.f;
	st.viewDepth = -8.0;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::setTopViewState(DfxQViewportState &st) {

	st.viewPhi	= 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX	 = 0.0f;
	st.viewPanY	= 0.0f;
}

void DfxQSceneViewport::setBottomViewState(DfxQViewportState &st) {

	st.viewPhi = 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::setLeftViewState(DfxQViewportState &st) {

	st.viewPhi = 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::setRightViewState(DfxQViewportState &st) {

	st.viewPhi = 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::setFrontViewState(DfxQViewportState &st) {

	st.viewPhi = 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::setBackViewState(DfxQViewportState &st) {

	st.viewPhi = 0.f;
	st.viewTheta = 0.f;
	st.viewDepth = 3.0f;
	st.viewPanX = 0.0f;
	st.viewPanY = 0.0f;
}

void DfxQSceneViewport::resetViews() {
	setPerspectiveViewState(stPerspective);
	setTopViewState(stTop);
	setBottomViewState(stBottom);
	setLeftViewState(stLeft);
	setRightViewState(stRight);
	setFrontViewState(stFront);
	setBackViewState(stBack);
}

std::vector<DfxColorField> DfxQSceneViewport::getMarqueeColors() {

	int sx = selectionMarquee->startX();
	int sy = selectionMarquee->startY();
	int ex = selectionMarquee->endX();
	int ey = selectionMarquee->endY();
	int cx = (sx <= ex) ? sx : ex;
	int cy = (sy >= ey) ? sy : ey;
	int tx = (sx >= ex) ? sx : ex;
	int ty = (sy <= ey) ? sy : ey;
	int w = tx - cx;
	int h = cy - ty;

	return renderer->colorAtPosition(cx, cy, w, h, size().height());
}

void DfxQSceneViewport::drawSelectionMarquee() {
	if (selectionMarquee->isActive()) {
		QBrush brush(QColor(255, 255, 255, 30));

		int width  = selectionMarquee->endX() - selectionMarquee->startX();
		int height = selectionMarquee->endY() - selectionMarquee->startY();
		QPainter p(this);
		p.setBrush(brush);
		p.drawRect(selectionMarquee->startX(), selectionMarquee->startY(), width, height);

	}
}

void DfxQSceneViewport::setViewLabel() {

	if (viewstate == ViewState::PERSPECTIVE) {
		overlay->currentViewString->setText("Perspective");
	}
	else if (viewstate == ViewState::TOP) {
		overlay->currentViewString->setText("Top");
	}
	else if (viewstate == ViewState::BOTTOM) {
		overlay->currentViewString->setText("Bottom");
	}
	else if (viewstate == ViewState::LEFT) {
		overlay->currentViewString->setText("Left");
	}
	else if (viewstate == ViewState::RIGHT) {
		overlay->currentViewString->setText("Right");
	}
	else if (viewstate == ViewState::FRONT) {
		overlay->currentViewString->setText("Front");
	}
	else if (viewstate == ViewState::BACK) {
		overlay->currentViewString->setText("Back");
	}
	else if (viewstate == ViewState::CAMERA) {
		DfxLxCameraHandler handler;
		auto rc = handler.fetch(cameraReferencePath);

		if (rc) {
			overlay->currentViewString->setText(handler.dataObject()->label().c_str());
		}
		else {
			overlay->currentViewString->setText("Camera");
		}
	}
}

void DfxQSceneViewport::switchToPerspective() {
	viewstate = ViewState::PERSPECTIVE;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToTop() {
	viewstate = ViewState::TOP;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToBottom() {
	viewstate = ViewState::BOTTOM;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToFront() {
	viewstate = ViewState::FRONT;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToBack() {
	viewstate = ViewState::BACK;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToLeft() {
	viewstate = ViewState::LEFT;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToRight() {
	viewstate = ViewState::RIGHT;
	cameraReferencePath = "";
	update();
}

void DfxQSceneViewport::switchToWireframeMode() {
	renderer->setShadingMode(DfxQViewportRenderInterface::ShadeMode::WIREFRAME);
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneViewport::switchToWireframeSolidMode() {
	renderer->setShadingMode(DfxQViewportRenderInterface::ShadeMode::WIREFRAME_SOLID_SHADE);
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneViewport::switchToSolidMode() {
	renderer->setShadingMode(DfxQViewportRenderInterface::ShadeMode::SOLID_SHADE);
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneViewport::populateCameraMenu() {
	overlay->camera->clear();
	
	deepsg::SgTraversalFunction cameraFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			if (node->getType() == deepsg::SgNodeType::SHAPE) {

				auto shapeNode = (deepsg::SgShapeNode*)node;

				if (shapeNode->shapeType() == DfxItemWidgetType::CAMERA) {

					auto refPath = node->referencePath();
					auto cam = DfxSpace::getInstance()->findAbsolute(refPath);

					std::string label = cam->label();
					QAction *act = new QAction(label.c_str());

					QObject::connect(act, &QAction::triggered, [this, refPath, node]() {
						viewstate = ViewState::CAMERA;
						cameraReferencePath = refPath;
						cameraNode = node;
						update();
					});

					overlay->camera->addAction(act);
				}

			}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(cameraFunc);
}

void DfxQSceneViewport::populateCameraFromViewMenu() {
	overlay->cameraFromView->clear();

	deepsg::SgTraversalFunction cameraFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->getType() == deepsg::SgNodeType::SHAPE) {

			auto shapeNode = (deepsg::SgShapeNode*)node;

			if (shapeNode->shapeType() == DfxItemWidgetType::CAMERA) {

				auto refPath = node->referencePath();
				auto cam = DfxSpace::getInstance()->findAbsolute(refPath);

				std::string label = cam->label();
				QAction *act = new QAction(label.c_str());

				QObject::connect(act, &QAction::triggered, [this, shapeNode, info, refPath, node]() {
					glm::vec3 pos;
					glm::quat rotQuat;
					glm::vec3 scl;
					glm::vec3 skew;
					glm::vec4 persp;

					glm::decompose(glm::inverse(viewMat), scl, rotQuat, pos, skew, persp);

					shapeNode->setPosition(glm::vec3(pos));
					shapeNode->setRotation(glm::vec3(-stPerspective.viewTheta, stPerspective.viewPhi, stPerspective.viewLamda) * -1.f);
			
					DfxGlobal::command::exec("updateScene");
				});

				overlay->cameraFromView->addAction(act);
			}

		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(cameraFunc);
}

void DfxQSceneViewport::initOpenGlContext() {
	initializeGL();
}

void DfxQSceneViewport::resizeEvent(QResizeEvent *event) {
	positionFrameInfomationOverlay();
	QOpenGLWidget::resizeEvent(event);
}

void DfxQSceneViewport::positionFrameInfomationOverlay() {
	auto x = (width() - frameOverlay->width()) - 10;
	auto y = height() - frameOverlay->height() - 10;

	frameOverlay->move(x, y);
}

void DfxQSceneViewport::updateFrameInfomation(QString info) {
	currentFrame->setText(info);
	frameOverlay->layout()->addWidget(currentFrame);
	positionFrameInfomationOverlay();
}

DfxQSceneViewport::TimeObserver::TimeObserver() {}

void DfxQSceneViewport::TimeObserver::update(fl::Any *any) {
	if (any->name() == "current_frame") {
		auto cs = DfxTimelineManager::getInstance()->currentFrame();

		ptr->updateFrameInfomation((std::string("Frame ") + std::to_string(cs)).c_str());
	}
}