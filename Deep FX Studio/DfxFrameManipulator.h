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

#include <QBoxlayout.h>
#include <QScrollArea>
#include <QPushButton.h>
#include <QPainter.h>
#include <QMouseEvent>
#include <cmath>
#include <vector>
#include <mutex>
#include <DfxScene.h>
#include "DeepStudioGlobal.h"
#include "DfxEnvelopeMap.h"
#include "DfxTime.h"
#include "DfxGlobal.h"



class DfxFrameManipulator : public QWidget {
	Q_OBJECT
private:
	int frameCount;
	int currentFrame;

	int pheight;
	int stepSize;

	QBrush cursorBrush;
	QPen   cursorPen;

	QBrush keyBrush;
	QPen   keyPen;

	QBrush keySelectedBrush;
	QPen   keySelectedPen;

	bool leftMouseButton;
	bool mouseMoved;

	std::mutex classLock;
private:
	void drawFrameScale();
	void drawFrameCursor();
	void drawKey(int frame);
	void drawKeys();
	void drawCachedOverlay();

	int mousePositionToFrame(int x);

	std::vector<int> keys;
protected:
	
	void paintEvent(QPaintEvent *event) override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	
public:
	DfxFrameManipulator(int fixedHeight, QWidget *parent = Q_NULLPTR);

	int getFrameCount();
	void setFrameCount(int );

	int getCurrentFrame();
	void setCurrentFrame(int);

	void setCanvasWidth(int width);

signals:
	void newFrameSet();
};
