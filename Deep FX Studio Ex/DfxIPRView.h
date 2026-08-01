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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QWidget>
#include <QMainWindow.h>
#include <QBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QComboBox>
#include <QPixmap>
#include <QVariant>
#include <FlSpaceObserver.h>
#include <QTimer>
#include <mutex>
#include <SgRootNode.h>
#include "DfxRenderSessionInterface.h"
#include "DfxRenderBuffer.h"
#include "DfxSpace.h"
#include "DfxQStyleScrollBar.h"
#include "DfxQComboBoxControl.h"

class DfxIPRView: public QMainWindow {
private:
	class CameraObserver :public fl::SpaceObserver {
	public:
		DfxIPRView *iprPtr;
		void update(fl::SpaceEventInfo info) override;
	};

private:
	CameraObserver *cameraObserver;
	void attachCameraObserver();
	void dettachCameraObserver();

private:
	bool iprActive;

	QTimer *refreshTimer;
	DfxRenderSessionInterface *renderSession;

	std::recursive_mutex _refreshLock;
private:
	QBoxLayout *mainLayout;
	QBoxLayout *headerLayout;
	QBoxLayout *headerLayoutLeft;
	QBoxLayout *headerLayoutRight;
	QBoxLayout *footerLayout;

	QWidget *iprWidget;

	QGraphicsView *graphicsView;
	QGraphicsScene *scene;
	QGraphicsPixmapItem *image_area;

	QPushButton *startStopIPR;
	QPushButton *refreshIPR;

	QComboBox *renderTarget;
	QLabel *renderTargetTitle;

	QComboBox *zoom;
	QLabel *zoomTitle;

	void styleBtns();

	void setupRenderTargets();

	void setupZoomCombobox(QComboBox *cb);

	void _start();
	void _stop();

	QImage _savedImage;

	int _currentRendererIndex;

	// DfxRenderOutput *_currentRenderOutput;
	DfxRenderBuffer *_currentRenderBuffer;

public:

	DfxIPRView(QWidget *widget = nullptr);
	~DfxIPRView();

public slots:
	void updateRenderers();
	void rendererChanged(int i);
	void refreshCameraList();
	void cameraChanged(int index);
	void refreshBuffer();
	void startStopIPRAction();
	void refreshIPRAction();
	void refreshImage();
	void resizeImage(int);
};