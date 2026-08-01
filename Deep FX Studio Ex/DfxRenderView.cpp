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

#include "DfxRenderView.h"
#include "DfxRenderOutput.h"

DfxRenderView::DfxRenderView(QWidget *widget) :QMainWindow(widget) {
	int btnSize = 32;

	mainLayout = new QVBoxLayout;
	headerLayout = new QHBoxLayout;
	headerLayoutLeft = new QHBoxLayout;
	headerLayoutRight = new QHBoxLayout;
	footerLayout = new QHBoxLayout;

	graphicsView = new QGraphicsView;
	scene = new QGraphicsScene;
	image_area = new QGraphicsPixmapItem;

	mainMenu = new QMenuBar;

	fileMenu = new QMenu("File");
	saveImageAction = new QAction("Save Image");

	stopRendering = new QPushButton("  Stop Render  ");

	renderTarget = new QComboBox;
	renderTargetTitle = new QLabel("Renderer:");

	aov = new QComboBox;
	aovTitle = new QLabel("AOV:");

	zoom = new QComboBox;
	zoomTitle = new QLabel("Zoom:");

	renderViewWidget = new QWidget;

	progressBar = new QProgressBar;

	_savedImage = QImage(540, 360, QImage::Format::Format_RGB888);
	_savedImage.fill(QColor(100, 100, 100));
	image_area->setPixmap(QPixmap::fromImage(_savedImage));

	scene->addItem(image_area);

	graphicsView->setScene(scene);
	style::styleQScrollBar(graphicsView->horizontalScrollBar());
	style::styleQScrollBar(graphicsView->verticalScrollBar());
	graphicsView->centerOn(image_area);

	DfxQComboBoxControl::styleComboBox(renderTarget);
	DfxQComboBoxControl::styleComboBox(aov);

	fileMenu->addAction(saveImageAction);

	mainMenu->addMenu(fileMenu);

	// setMenuBar(mainMenu);

	styleBtns();

	setupZoomCombobox(zoom);
	zoom->setCurrentIndex(10);

	aov->setMinimumWidth(100);

	progressBar->setMinimum(0);
	progressBar->setMaximum(100);
	progressBar->setValue(0);
	progressBar->setFixedWidth(150);

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

	headerLayoutRight->addWidget(progressBar);

	headerLayoutLeft->addWidget(stopRendering);
	headerLayoutLeft->addSpacing(10);
	headerLayoutLeft->addWidget(renderTargetTitle);
	headerLayoutLeft->addWidget(renderTarget);
	headerLayoutLeft->addSpacing(6);
	headerLayoutLeft->addWidget(aovTitle);
	headerLayoutLeft->addWidget(aov);
	headerLayoutLeft->addSpacing(6);
	headerLayoutLeft->addWidget(zoomTitle);
	headerLayoutLeft->addWidget(zoom);

	headerLayout->addLayout(headerLayoutLeft);
	headerLayout->addLayout(headerLayoutRight);

	mainLayout->addLayout(headerLayout);
	mainLayout->addWidget(graphicsView);
	mainLayout->addLayout(footerLayout);

	renderViewWidget->setLayout(mainLayout);
	setCentralWidget(renderViewWidget);

	_currentRendererIndex = 0;
	_currentAOVIndex = 0;

	_currentRenderOutput = nullptr;
	_currentRenderBuffer = nullptr;

	updateRenderers();

	connect(DfxRenderHub::getInstance(), &DfxRenderHub::renderNodesUpdated, this, &DfxRenderView::updateRenderers);

	connect(renderTarget, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxRenderView::rendererChanged);
	connect(aov, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxRenderView::bufferUpdated);
	connect(stopRendering, &QPushButton::clicked, this, &DfxRenderView::stopRender);
	connect(zoom, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxRenderView::resizeImage);

}

DfxRenderView::~DfxRenderView() {

}

void DfxRenderView::styleBtns() {
	stopRendering->setStyleSheet(
		stopRendering->styleSheet() +
		style::leftButtonSty() +
		style::rightButtonSty() +
		style::hoverSty() +
		style::hoverPressdSty());
}

void DfxRenderView::setupZoomCombobox(QComboBox *cb) {

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

void DfxRenderView::rendererChanged(int index) {
	_currentRendererIndex = index;
	updateBuffers();
}

void DfxRenderView::updateRenderers() {
	renderTarget->clear();
	renderTarget->addItem("None");

	for (int i = 0; i < DfxRenderHub::getInstance()->renderNodeCount(); i++) {
		auto rnode = DfxRenderHub::getInstance()->renderNode(i);
		renderTarget->addItem(rnode->renderOutput()->rendererName().c_str());
	}

	if (_currentRendererIndex != 0)renderTarget->setCurrentIndex(_currentRendererIndex);
	updateBuffers();
}

void DfxRenderView::updateBuffers() {
	
	aov->clear();

	if (_currentRenderOutput != nullptr) {
		QObject::disconnect(_currentRenderOutput, &DfxRenderOutput::buffersUpdated, this, &DfxRenderView::updateBuffers);
		QObject::disconnect(_currentRenderOutput, &DfxRenderOutput::buffersCleared, this, &DfxRenderView::buffersCleared);
		QObject::disconnect(_currentRenderOutput, &DfxRenderOutput::progressUpdated, this, &DfxRenderView::updateProgress);
		QObject::disconnect(_currentRenderOutput, &DfxRenderOutput::aboutToUpdateBuffer, this, &DfxRenderView::releaseCurrentRenderBuffer);
	}

	if (_currentRendererIndex > 0) {
		
		for (int i = 0; i < DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->bufferCount(); i++) {
			auto buffer = DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->getBuffer(i);
			aov->addItem(buffer->bufferName().c_str());
		}

		_currentRenderOutput = DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput();
		QObject::connect(_currentRenderOutput, &DfxRenderOutput::buffersUpdated, this, &DfxRenderView::updateBuffers);
		QObject::connect(_currentRenderOutput, &DfxRenderOutput::buffersCleared, this, &DfxRenderView::buffersCleared);
		QObject::connect(_currentRenderOutput, &DfxRenderOutput::progressUpdated, this, &DfxRenderView::updateProgress);
	
		progressBar->setValue(_currentRenderOutput->progress());

	}
	else {
		_currentRenderOutput = nullptr;
		_savedImage = QImage(540, 360, QImage::Format::Format_RGB888);
		resizeImage(0);
	}

	_currentAOVIndex = 0;

	refreshBuffer();
}

void DfxRenderView::bufferUpdated(int i) {
	_currentAOVIndex = i;
	refreshBuffer();
}

void DfxRenderView::buffersCleared() {
	_currentRenderBuffer = nullptr;
}

void DfxRenderView::refreshBuffer() {
	
	if(_currentRenderBuffer != nullptr ) {

		QObject::disconnect(_currentRenderBuffer, &DfxRenderBuffer::updated, this, &DfxRenderView::refreshBuffer);
		_currentRenderBuffer = nullptr;
	}

	if (_currentRendererIndex > 0) {

		if (DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->bufferCount() > 0) {

			_currentRenderBuffer = DfxRenderHub::getInstance()->renderNode(_currentRendererIndex - 1)->renderOutput()->getBuffer(_currentAOVIndex);

			scene->removeItem(image_area); delete image_area;
			image_area = new QGraphicsPixmapItem;
			scene->addItem(image_area);

			image_area->setPixmap(QPixmap::fromImage(_currentRenderBuffer->getImage()));

			resizeImage(0);
			QObject::connect(_currentRenderBuffer, &DfxRenderBuffer::updated, this, &DfxRenderView::refreshBuffer);

		}
	}

}

void DfxRenderView::updateProgress(int i) {
	progressBar->setValue(i);
}

void DfxRenderView::stopRender() {
	if (_currentRenderOutput != nullptr) _currentRenderOutput->stopRenderer();
}

void DfxRenderView::resizeImage(int) {

	graphicsView->setSceneRect(image_area->boundingRect());
	graphicsView->resetTransform();
	graphicsView->scale(zoom->currentData().toFloat(), zoom->currentData().toFloat());
	
}

void DfxRenderView::releaseCurrentRenderBuffer() {
	if (_currentRenderBuffer != nullptr) {
		QObject::disconnect(_currentRenderBuffer, &DfxRenderBuffer::updated, this, &DfxRenderView::refreshBuffer);
		_currentRenderBuffer = nullptr;
	}
}
