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

#include <QImage>
#include <QtWidgets\QOpenglWidget>
#include <QKeySequence>
#include "DfxShortcutManager.h"

class DfxAbstractViewport: public QOpenGLWidget
{
private:
	QKeySequence zoomSequence;
	QKeySequence panSequence;
	QKeySequence rotateSequence;
public:
	DfxAbstractViewport();
	virtual ~DfxAbstractViewport();

	virtual void reset() = 0;
	virtual QImage *capture() = 0;

	void setZoomSequence(QKeySequence );
	QKeySequence getZoomSequence();

	void setPanSequence(QKeySequence);
	QKeySequence getPanSequence();
	
	void setRotateSequence(QKeySequence);
	QKeySequence getRotateSequence();

public:

	virtual void renderSolidRenderMode() = 0;
	virtual void renderSolidWireRenderMode() = 0;
	virtual void renderWireRenderMode() = 0;
};