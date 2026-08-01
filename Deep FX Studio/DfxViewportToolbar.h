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

#include <DfxCoreApplication.h>
#include <QObject>
#include <QToolbar>
#include <QAction>
#include <QIcon>

class DfxViewportToolbar: public QObject {
	Q_OBJECT
private:
	QToolBar *_bar;

	QAction *translateAction;
	QAction *rotateAction;
	QAction *scaleAction;

	QAction *planePrimitiveAction;
	QAction *boxPrimitiveAction;
	QAction *spherePrimitiveAction;
	QAction *torusPrimitiveAction;

	QAction *lastTransformAction;
public:

	DfxViewportToolbar();

	static void addToolbar();
	QToolBar *getToolBar();
public slots:

	void translateSlot();
	void scaleSlot();
	void rotateSlot();

	void planeSlot();
	void boxSlot();
	void sphereSlot();
	void torusSlot();
};