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

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QBoxLayout>
#include <QMenubar>
#include <QMenu>
#include <QDialog>
#include <QThread>
#include <thread>
#include <QProgressBar>
#include <QTime>
#include "DfxRenderInterface.h"
#include "DfxLayerManager.h"
#include "DfxRenderStats.h"


class DfxRenderThreadObject: public QThread {
	Q_OBJECT
private:
	DfxRenderInterface *rinterface;
	bool fAbort;
public:
	
	DfxRenderThreadObject(DfxRenderInterface *rinterface);
	void run(void) override;
	void abort();
signals:
	void updateProgress(int val);
	void update(QPixmap pmap);
	void updateRenderTime(int a, int b);
};

class DfxRenderViewer: public QMainWindow {
	Q_OBJECT
private:
	QBoxLayout *mainLayout;
	QBoxLayout *buttonLayout;
	QBoxLayout *buttonLayout00;
	QBoxLayout *buttonLayout01;
	
	QScrollArea *imageArea;
	QLabel *image;

	QPushButton *fileMenuButton;
	QMenu *fileMenu;
	QAction *saveRender;

	QPushButton *zoomButton;
	QMenu *zoomMenu;
	QAction *zoom10;
	QAction *zoom20;
	QAction *zoom30;
	QAction *zoom40;
	QAction *zoom50;
	QAction *zoom80;
	QAction *zoom100;
	QAction *zoom200;
	QAction *zoom300;
	QAction *zoom400;
	QAction *zoom500;

	QLabel *renderTimeLabel;
	QProgressBar *progress;
	QPushButton  *abort;

	DfxRenderInterface *rinterface;

	DfxRenderThreadObject *render_thread;
private:

	void setupRender();
	void render();
public:
	DfxRenderViewer();
	virtual ~DfxRenderViewer();
	
	void setRenderInterface(DfxRenderInterface *);
	void startRender();

	bool isFinished();
public slots:

	void updateRenderOutput(QPixmap pmap);
	void updateProgress(int value);
	void abortRender(bool);
	void updateTimeStats(int time, int last);
};