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
#include <QMenuBar>
#include <QMenu>
#include <QProgressBar>
#include "DfxSpace.h"
#include "DfxQPushButtonStyles.h"
#include "DfxLuxCoreRenderSession.h"
#include "DfxQStyleScrollBar.h"
#include "DfxQComboBoxControl.h"
#include "DfxRenderHub.h"
#include "DfxRenderBuffer.h"

class DfxRenderView : public QMainWindow {
	Q_OBJECT
private:

	QBoxLayout *mainLayout;
	QBoxLayout *headerLayout;
	QBoxLayout *headerLayoutLeft;
	QBoxLayout *headerLayoutRight;
	QBoxLayout *footerLayout;

	QWidget *renderViewWidget;

	QGraphicsView *graphicsView;
	QGraphicsScene *scene;
	QGraphicsPixmapItem *image_area;

	qreal gview_width;
	qreal gview_height;

	QPushButton *stopRendering;

	QLabel *renderTargetTitle;
	QComboBox *renderTarget;

	QLabel *aovTitle;
	QComboBox *aov;

	QLabel *zoomTitle;
	QComboBox *zoom;
	
	QMenuBar *mainMenu;

	QMenu *fileMenu;
	QAction *saveImageAction;

	QProgressBar *progressBar;

	int _currentRendererIndex;
	int _currentAOVIndex;

	void styleBtns();

	QImage _savedImage;

private:
	DfxRenderOutput *_currentRenderOutput;
	DfxRenderBuffer *_currentRenderBuffer;

private:
	void setupZoomCombobox(QComboBox *cb);

public:

	DfxRenderView(QWidget *widget = nullptr);
	~DfxRenderView();

public slots:

	void updateRenderers();
	void rendererChanged(int i);
	void updateBuffers();
	void bufferUpdated(int i);
	void buffersCleared();
	void refreshBuffer();
	void updateProgress(int i);
	void stopRender();
	void resizeImage(int);
	void releaseCurrentRenderBuffer();
};