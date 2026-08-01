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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxIPRView.h"
#include "DfxRenderHub.h"
#include "DfxRenderNodeBase.h"
#include "DfxRenderOutput.h"
#include "DfxRenderSessionInterface.h"

DfxIPRView::DfxIPRView(QWidget *widget) :QMainWindow(widget) {
	int btnSize = 32;

	mainLayout = new QVBoxLayout;
	headerLayout = new QHBoxLayout;
	headerLayoutLeft = new QHBoxLayout;
	headerLayoutRight = new QHBoxLayout;
	footerLayout = new QHBoxLayout;

	graphicsView = new QGraphicsView;
	scene = new QGraphicsScene;
	image_area = new QGraphicsPixmapItem;

	startStopIPR = new QPushButton(QPixmap(":/ipr/start_ipr"),"");
	refreshIPR   = new QPushButton(QPixmap(":/ipr/refresh_ipr"), "");

	renderTarget = new QComboBox;
	renderTargetTitle = new QLabel("Renderer:");

	zoom = new QComboBox;
	zoomTitle = new QLabel("Zoom:");
	setupZoomCombobox(zoom);
	zoom->setCurrentIndex(10);

	iprWidget = new QWidget;

	_savedImage = QImage(540, 360, QImage::Format::Format_RGB888);
	_savedImage.fill(QColor(100, 100, 100));
	image_area->setPixmap(QPixmap::fromImage(_savedImage));

	renderSession = nullptr;
	refreshTimer = nullptr;
	_currentRenderBuffer = nullptr;

	scene->addItem(image_area);

	startStopIPR->setFixedSize(btnSize, btnSize);
	startStopIPR->setIconSize(QSize(20, 20));
	refreshIPR->setFixedSize(btnSize, btnSize);
	refreshIPR->setIconSize(QSize(20, 20));

	graphicsView->setScene(scene);
	style::styleQScrollBar(graphicsView->horizontalScrollBar());
	style::styleQScrollBar(graphicsView->verticalScrollBar());
	graphicsView->centerOn(image_area);
	
	DfxQComboBoxControl::styleComboBox(renderTarget);
	DfxQComboBoxControl::styleComboBox(zoom);

	styleBtns();

	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	headerLayout->setContentsMargins(0, 0, 0, 5);
	headerLayout->setSpacing(0);

	headerLayoutLeft->setContentsMargins(8, 0, 0, 0);
	headerLayoutLeft->setSpacing(2);
	headerLayoutLeft->setAlignment(Qt::AlignLeft);

	headerLayoutRight->setContentsMargins(0, 0, 8, 0);
	headerLayoutRight->setSpacing(0);
	headerLayoutRight->setAlignment(Qt::AlignRight);

	footerLayout->setMargin(0);
	footerLayout->setSpacing(0);

	headerLayoutRight->addWidget(refreshIPR);
	headerLayoutRight->setSpacing(2);
	headerLayoutRight->addWidget(startStopIPR);

	headerLayoutLeft->addWidget(renderTargetTitle);
	headerLayoutLeft->addWidget(renderTarget);
	headerLayoutLeft->addSpacing(6);
	headerLayoutLeft->addWidget(zoomTitle);
	headerLayoutLeft->addWidget(zoom);

	headerLayout->addLayout(headerLayoutLeft);
	headerLayout->addLayout(headerLayoutRight);

	mainLayout->addLayout(headerLayout);
	mainLayout->addWidget(graphicsView);
	mainLayout->addLayout(footerLayout);

	iprWidget->setLayout(mainLayout);

	setupRenderTargets();

	setCentralWidget(iprWidget);

	refreshCameraList();
	attachCameraObserver();

	iprActive = false;

	_currentRendererIndex = 0;

	updateRenderers();

	QObject::connect(DfxRenderHub::getInstance(), &DfxRenderHub::renderNodesUpdated, this, &DfxIPRView::updateRenderers);

	QObject::connect(renderTarget, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxIPRView::rendererChanged);
	QObject::connect(startStopIPR, &QPushButton::clicked, this, &DfxIPRView::startStopIPRAction);
	QObject::connect(refreshIPR, &QPushButton::clicked, this, &DfxIPRView::refreshIPRAction);
	QObject::connect(zoom, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxIPRView::resizeImage);

	// QObject::connect(cameras, qOverload<int>(&QComboBox::currentIndexChanged), this, &DfxIPRView::cameraChanged);
}

DfxIPRView::~DfxIPRView() {
	if (renderSession != nullptr) delete renderSession;
	dettachCameraObserver();
}

void DfxIPRView::styleBtns() {
	QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 4px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";


	startStopIPR->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	refreshIPR->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
}

void DfxIPRView::setupZoomCombobox(QComboBox *cb) {

	cb->clear();

	cb->addItem("1%", 0.01f);
	cb->addItem("10%", 0.1f);
	cb->addItem("20%", 0.2f);
	cb->addItem("30%", 0.3f);
	cb->addItem("40%", 0.4f);
	cb->addItem("50%", 0.5f);
	cb->addItem("60%", 0.6f);
	cb->addItem("70%", 0.7f);
	cb->addItem("80%", 0.8f);
	cb->addItem("90%", 0.9f);
	cb->addItem("100%", 1.0f);
	cb->addItem("110%", 1.1f);
	cb->addItem("120%", 1.2f);
	cb->addItem("130%", 1.3f);
	cb->addItem("140%", 1.4f);
	cb->addItem("150%", 1.5f);
	cb->addItem("160%", 1.6f);
	cb->addItem("170%", 1.7f);
	cb->addItem("180%", 1.8f);
	cb->addItem("190%", 1.9f);
	cb->addItem("200%", 2.f);

}

void DfxIPRView::rendererChanged(int index) {
	_currentRendererIndex = index;
	//updateBuffers();
}

void DfxIPRView::updateRenderers() {
	renderTarget->clear();
	renderTarget->addItem("None");

	for (int i = 0; i < DfxRenderHub::getInstance()->renderNodeCount(); i++) {
		auto rnode = DfxRenderHub::getInstance()->renderNode(i);
		renderTarget->addItem(rnode->renderOutput()->rendererName().c_str());
	}

	if (_currentRendererIndex != 0)renderTarget->setCurrentIndex(_currentRendererIndex);
	//updateBuffers();
}

void DfxIPRView::setupRenderTargets() {
	auto any = DfxSpace::getInstance()->findAbsolute("~/RENDER/render_targets");

	if (any != nullptr) {
		if (any->type() == fl::type::DATA_OBJECT) {
			for (auto p : any->data<fl::DataObject>().properties()) {
				renderTarget->addItem(p->label().c_str(), QVariant(p->data<int>()));
			}
		}
	}
}

void DfxIPRView::attachCameraObserver() {
	cameraObserver = new CameraObserver;
	cameraObserver->iprPtr = this;

	DfxSpace::getInstance()->addObserver(cameraObserver);
}

void DfxIPRView::dettachCameraObserver() {
	delete cameraObserver;
}

void DfxIPRView::refreshCameraList() {
	/*cameras->clear();

	auto any = DfxSpace::getInstance()->findAbsolute("~/CAMS");

	if (any != nullptr) {
		if (any->type() == fl::type::DATA_OBJECT) {
			for (auto p : any->data<fl::DataObject>().properties()) {
				auto path = p->absolutePath();
				cameras->addItem(p->label().c_str(), QVariant(QString(path.c_str())));
			}
		}
	}*/
}

void DfxIPRView::startStopIPRAction() {
	
	if (!iprActive) {
		_start();
	}
	else {
		_stop();
	}
}

void DfxIPRView::refreshIPRAction() {

	if (iprActive) {
		_stop();
		_start();
	}
}

void DfxIPRView::refreshImage() {
	if (renderSession != nullptr) {
		QImage img = renderSession->output();
		image_area->setPixmap(QPixmap::fromImage(img));
	}
}

void DfxIPRView::cameraChanged(int index) {
	/*if (renderSession != nullptr) {
		if (renderTarget->currentData().toInt() == DfxRenderTarget::LUXCORE) {
			if (((DfxLuxCoreRenderSession*)renderSession)->isStarted()) {
				DfxGlobal::console::println("Camera index updated...");
				renderSession->setCameraPath(cameras->currentData().toString().toStdString());
				((DfxLuxCoreRenderSession*)renderSession)->updateCamera();
			}
		}
	}*/
}

void DfxIPRView::refreshBuffer() {
	if (auto lock = std::unique_lock<std::recursive_mutex>(_refreshLock, std::try_to_lock)) {
		if (_currentRendererIndex > 0) {

			if (DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->bufferCount() > 0) {

				QApplication::processEvents();
				_currentRenderBuffer = DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->getBuffer(0);

				scene->removeItem(image_area); delete image_area;
				image_area = new QGraphicsPixmapItem;
				scene->addItem(image_area);

				image_area->setPixmap(QPixmap::fromImage(_currentRenderBuffer->getImage()));

				resizeImage(0);

			}
		}
	}
}

void DfxIPRView::resizeImage(int) {
	graphicsView->setSceneRect(image_area->boundingRect());
	graphicsView->resetTransform();
	graphicsView->scale(zoom->currentData().toFloat(), zoom->currentData().toFloat());
}

void DfxIPRView::_start() {

	auto space = DfxSpace::getInstance();

	if (DfxRenderHub::getInstance()->renderNodeCount() > 0 && _currentRendererIndex > 0) {

		if (renderSession != nullptr) delete renderSession;

		auto renderNode = DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1);
		renderSession = renderNode->renderSession();

		if (renderSession != nullptr) {
			renderSession->enableInteractivePreview();

			if (!renderSession->init()) {
				renderSession->clear();
				delete renderSession;

				DfxGlobal::console::println("Failed to initialize render session");
				return;
			}

			if (!renderSession->loadScene(deepsg::SgRootNode::getInstance())) {
				renderSession->clear();
				delete renderSession;

				DfxGlobal::console::println("Render session failed to load scene");
				return;
			}

			if (!renderSession->prepareSession()) {
				renderSession->clear();
				delete renderSession;

				DfxGlobal::console::println("Failed to prepare render session");
				return;
			}

			refreshTimer = new QTimer();

			QObject::connect(refreshTimer, &QTimer::timeout, [this]() { refreshBuffer(); });

			renderSession->startRenderSession();
			refreshTimer->start(200);

			startStopIPR->setIcon(QIcon(QPixmap(":/ipr/stop_ipr")));
			iprActive = true;	

		}

	}
	
}

void DfxIPRView::_stop() {

	if (refreshTimer != nullptr) {
		refreshTimer->stop();
		delete refreshTimer;
		refreshTimer = nullptr;
	}

	if (renderSession != nullptr) {
		renderSession->stopRenderSession();
		renderSession->clear();
	}

	if (_currentRenderBuffer != nullptr) {

		QObject::disconnect(_currentRenderBuffer, &DfxRenderBuffer::updated, this, &DfxIPRView::refreshBuffer);
		_currentRenderBuffer = nullptr;
	}

	startStopIPR->setIcon(QIcon(QPixmap(":/ipr/start_ipr")));
	iprActive = false;
}

// Camera Observer
void DfxIPRView::CameraObserver::update(fl::SpaceEventInfo info) {

	if (info.type == fl::SpaceEventType::POST_MOVE_EVENT ){
		auto path = info.move_destination->absolutePath();

		if (path == "~/CAMS") {
			iprPtr->refreshCameraList();
		}
	}
	else if (info.type == fl::SpaceEventType::POST_REMOVE_EVENT) {
		auto path = info.remove_source->absolutePath();

		if (path == "~/CAMS") {
			iprPtr->refreshCameraList();
		}
	}
}