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

#include <iostream>
#include "DfxFrameManipulator.h"

using namespace std;

DfxFrameManipulator::DfxFrameManipulator(int height, QWidget *parent) : QWidget(parent) {

	pheight = height;
	setFixedHeight(height);

	cursorBrush.setColor(QColor(0, 0, 0));
	cursorBrush.setStyle(Qt::BrushStyle::SolidPattern);

	cursorPen.setColor(QColor(0, 0, 0));
	cursorPen.setStyle(Qt::PenStyle::SolidLine);
	cursorPen.setWidth(2);

	keyBrush.setColor(QColor(95, 113, 135));
	keyBrush.setStyle(Qt::BrushStyle::SolidPattern);

	keyPen.setColor(QColor(130, 141, 152));
	keyPen.setStyle(Qt::PenStyle::SolidLine);
	keyPen.setWidth(2);

	keySelectedBrush.setColor(QColor(171, 106, 43));
	keySelectedBrush.setStyle(Qt::BrushStyle::SolidPattern);

	keySelectedPen.setColor(QColor(176, 147, 80));
	keySelectedPen.setStyle(Qt::PenStyle::SolidLine);
	keySelectedPen.setWidth(2);

	frameCount = 120;
	currentFrame = 20;

	leftMouseButton = false;
	mouseMoved = false;

	keys.push_back(10);
	keys.push_back(50);
	keys.push_back(100);
	keys.push_back(88);
	
	setMouseTracking(true);
}

void DfxFrameManipulator::setCanvasWidth(int width) {

	if (frameCount >= 100) {
		stepSize = std::ceil(width / 100);
	}
	else if (frameCount < 100 && frameCount >= 50) {
		stepSize = std::ceil(width / 50);
	}
	else if (frameCount < 50 && frameCount >= 20) {
		stepSize = std::ceil(width / 20);
	}
	else if (frameCount < 20) {
		stepSize = std::ceil(width / 10);
	}
	else if (frameCount <= 5) {
		stepSize = std::ceil(width / 5);
	}

	setGeometry(QRect(0, 0, (frameCount + 4)*stepSize, pheight * 0.5));
}

void DfxFrameManipulator::drawCachedOverlay() {

	QPainter painter(this);

	painter.setBrush(QColor(0, 0, 128));
	painter.setPen(Qt::blue);

	for (int inx = 1; inx <= frameCount; inx++) {
		if(DeepStudioGlobal::getInstance()->isFrameCached(inx))
			painter.drawRect(inx * stepSize, 0, stepSize, 10);
	}
}

void DfxFrameManipulator::drawFrameScale() {

	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::white);

	int pwidth = size().width();

	for (int inx = 1; inx <= frameCount; inx++) {

		int tHeight = 0;
		bool drawText = false;

		if (inx == 1) {
			tHeight = 16;
			drawText = true;
		}
		else if (inx % 10 == 0) {
			tHeight = 16;
			drawText = true;
		}
		else
			tHeight = 8;

		painter.drawLine(inx * stepSize, pheight, inx * stepSize, pheight - tHeight);

		if (drawText)
			painter.drawText(QPoint(inx * stepSize, pheight - tHeight - 1), QString::number(inx));

	}
}

void DfxFrameManipulator::drawFrameCursor() {

	int singeleFrame = stepSize;
	int halfStep = stepSize * .5f;

	QPainter painter(this);
	painter.setBrush(cursorBrush);
	painter.setPen(Qt::NoPen);

	painter.drawRect((singeleFrame * currentFrame) - halfStep, 0, stepSize, 16);

	painter.setPen(Qt::white);
	painter.drawText(QPoint((singeleFrame * currentFrame), pheight - 16 - 1), QString::number(currentFrame));

	painter.setPen(cursorPen);
	painter.drawLine(currentFrame * stepSize, pheight, currentFrame * stepSize, pheight - 16);
}

void DfxFrameManipulator::drawKey(int frame) {
	int singeleFrame = stepSize;
	int halfStep = (stepSize - 5) * .5f;

	QPainter painter(this);
	painter.setBrush(keyBrush);
	painter.setPen(keyPen);

	painter.drawRect((singeleFrame * frame) - halfStep, pheight - 12, stepSize - 5, 12);

}
void DfxFrameManipulator::drawKeys() {
	
	auto sel = DfxScene::getInstance()->getSelectedItems();
	
	if (sel.size() > 0) {

		DfxViewItem *itemPtr = sel[0];

		auto envPtr = DfxEnvelopeMap::getInstance()->findEnvelope(itemPtr->getInstanceID(), DfxViewItem::position);

		if (envPtr != nullptr) {
			for (int inx = 1; inx <= frameCount; inx++) {

				if (envPtr->hasKey(inx)) 
					drawKey(inx);
			}
		}
	}
}

void DfxFrameManipulator::paintEvent(QPaintEvent *event) {
	drawCachedOverlay();
	drawFrameScale();
	drawFrameCursor();
	drawKeys();
}

int DfxFrameManipulator::mousePositionToFrame(int x) {

	int frame = x / stepSize;

	if (frame <= 0) frame = 1;
	if (frame > frameCount) frame = frameCount;

	return frame;
}

void DfxFrameManipulator::mousePressEvent(QMouseEvent *evt) {

	if (evt->button() == Qt::MouseButton::LeftButton) {
		leftMouseButton = true;

		if (leftMouseButton) {
			setCurrentFrame(mousePositionToFrame(evt->x()));
		}
	}
	
}

void DfxFrameManipulator::mouseReleaseEvent(QMouseEvent *evt) {
	
	leftMouseButton = false;
	mouseMoved = false;
}

void DfxFrameManipulator::mouseMoveEvent(QMouseEvent *evt) {

	if (leftMouseButton) {
		mouseMoved = true;

		setCurrentFrame(mousePositionToFrame(evt->x()));
	}

}

int DfxFrameManipulator::getFrameCount() {
	return frameCount;
}

void DfxFrameManipulator::setFrameCount(int fc) {
	while (!classLock.try_lock());

	if (fc <= 0) fc = 1;

	if (currentFrame > fc) setCurrentFrame(fc);

	frameCount = fc;
	DfxTime::getInstance()->setTotalFrames(fc);
	setGeometry(QRect(0, 0, (frameCount + 4)*stepSize, pheight * 0.5));
	update();

	DfxGlobal::command::exec("updateViewport");

	classLock.unlock();
}

int DfxFrameManipulator::getCurrentFrame() {
	return currentFrame;
}

void DfxFrameManipulator::setCurrentFrame(int cf) {
	while (!classLock.try_lock());

	currentFrame = cf;
	DfxTime::getInstance()->setNewFrame(cf);
	update();
	newFrameSet();
	DfxGlobal::command::exec("updateViewport");

	classLock.unlock();
}

