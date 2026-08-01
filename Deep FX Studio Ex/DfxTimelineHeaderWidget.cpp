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

#include "DfxTimelineHeaderWidget.h"
#include "DfxSimulationManager.h"
#include "DfxTimelineManager.h"
#include "DfxMode.h"

DfxTimelineHeaderWidget::DfxTimelineHeaderWidget(int fs, QWidget *parent):QWidget(parent) {

	_minFrameScale = fs;

	_trackWidth  = 200;
	_trackHeight = 100;

	_mousePressed = false;
	_mouseMoved = false;

	mousePressframe = 0;

	setFixedHeight(constants::header_height);

	frameIndicatorPtr = nullptr;

	auto currentFrameAny  = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
	_currentFrameObserver = new CurrentFrameObserver;
	_currentFrameObserver->headerPtr = this;
	currentFrameAny->addObserver(_currentFrameObserver);

	setStyleSheet({ "background-color: grey;" });

	QObject::connect(DfxSimulationManager::getInstance(), &DfxSimulationManager::resetTriggered, this, &DfxTimelineHeaderWidget::updateHeader);
}

DfxTimelineHeaderWidget::~DfxTimelineHeaderWidget() {
	delete _currentFrameObserver;
}

void DfxTimelineHeaderWidget::setTrackWidth(int w) {
	_trackWidth = w;
}

int  DfxTimelineHeaderWidget::trackWidth() const {
	return _trackWidth;
}

void DfxTimelineHeaderWidget::setTrackHeight(int h) {
	_trackHeight = h;
}

int DfxTimelineHeaderWidget::trackHeight() const {
	return _trackHeight;
}

void DfxTimelineHeaderWidget::drawSimulationState(QPainter &painter, int stepSize) {

	if (DfxSimulationManager::getInstance()->needsReset()) {
		painter.setBrush(QColor(255, 153, 85));
		painter.setPen(QColor(255, 102, 0));
	}
	else {
		painter.setBrush(QColor(0, 0, 120));
		painter.setPen(Qt::blue);
	}

	for (int inx = 1; inx < DfxSimulationManager::getInstance()->stepsCovered(); inx++) {
		painter.drawRect(inx * stepSize, 0, stepSize, 5);
	}
}

void DfxTimelineHeaderWidget::paintEvent(QPaintEvent *event) {

	// int frameCount = _endFrame - _startFrame;
	int stepSize = trackWidth() / _minFrameScale;
	int pheight = constants::header_height;

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	QPainter painter(this);
	
	// Simulation State
	drawSimulationState(painter, stepSize);

	painter.setBrush(Qt::SolidPattern);
	painter.setPen(Qt::white);

	int frame_counter = _startFrame;
	for (int inx = 1; frame_counter <= _endFrame; inx++) {

		int tHeight = 0;
		bool drawText = false;

		if (frame_counter == 0) {
			tHeight = 8;
			drawText = true;
		}
		else if (frame_counter % 10 == 0) {
			tHeight = 8;
			drawText = true;
		}
		else
			tHeight = 4;

		painter.drawLine(inx * stepSize, pheight, inx * stepSize, pheight - tHeight);

		if (drawText)
			painter.drawText(QPoint(inx * stepSize, pheight - tHeight - 1), QString::number(frame_counter));

		frame_counter++;
	}

	updateIndicator();
	QWidget::paintEvent(event);
}

void DfxTimelineHeaderWidget::setMinFrameScale(int scl) {
	_minFrameScale = scl;
}

int DfxTimelineHeaderWidget::minScaleFrame() {
	return _minFrameScale;
}

int DfxTimelineHeaderWidget::length() const{
	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	return (stepSize * (_endFrame - _startFrame)) + (stepSize * 3);
}

QSize DfxTimelineHeaderWidget::sizeHint() const {
	return QSize(length(), constants::header_height);
}

void DfxTimelineHeaderWidget::setFrameIndicator(DfxTimelineFrameIndicator *indicator) {
	frameIndicatorPtr = indicator;
}

void DfxTimelineHeaderWidget::updateIndicator() {

	auto currentFrame = DfxTimelineManager::getInstance()->currentFrame();
	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;

	int xpos = stepSize * ((currentFrame + 1) - _startFrame);

	QPainter painter(this);

	QPen pen(QColor(255, 153, 83));
	pen.setWidth(2);

	painter.setBrush(QBrush(QColor(255, 153, 83)));
	painter.setPen(pen);

	painter.drawRoundRect(xpos - (stepSize * .5), constants::header_height - 8, stepSize, 8);

	if (frameIndicatorPtr != nullptr) {
		frameIndicatorPtr->setX(xpos);
		frameIndicatorPtr->update();
	}
}

void DfxTimelineHeaderWidget::mousePressEvent(QMouseEvent *event) {

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	int stepSize = trackWidth() / _minFrameScale;
	int offset = (event->pos().x() / stepSize) - 1;
	mousePressframe = std::max(std::min(_startFrame + offset, _endFrame), _startFrame);

	_mousePressed = true;
	
}

void DfxTimelineHeaderWidget::mouseMoveEvent(QMouseEvent *event) {

	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame = DfxTimelineManager::getInstance()->lastFrame();

	if (_mousePressed) {
		int stepSize = trackWidth() / _minFrameScale;
		int offset = (event->pos().x() / stepSize) - 1;
		mousePressframe = std::max(std::min(_startFrame + offset, _endFrame), _startFrame);

		DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
		DfxTimelineManager::getInstance()->goToFrame(mousePressframe);
		DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);

		update();
		_mouseMoved = true;
	}
}

void DfxTimelineHeaderWidget::mouseReleaseEvent(QMouseEvent *event) {

	if (!_mouseMoved) {

		DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::UPDATING);
		DfxTimelineManager::getInstance()->goToFrame(mousePressframe);
		DfxMode::getInstance()->setTimelineMode(DfxMode::TimelineMode::NONE);

		update();
	}

	_mouseMoved = false;
	_mousePressed = false;
}

void DfxTimelineHeaderWidget::updateFrameScale(int value) {
	_minFrameScale = value;
	update();
}

void DfxTimelineHeaderWidget::updateHeader() {
	update();
}

void DfxTimelineHeaderWidget::CurrentFrameObserver::update(fl::Any *any) {
	headerPtr->update();
}