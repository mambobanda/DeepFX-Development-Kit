
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

#include <QMainWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QTabWidget>
#include <DfxCommon.h>
#include "DfxQSceneViewViewportWrangler.h"
#include "DfxQViewportCommon.h"
#include "DfxQViewportState.h"
#include "DfxQViewportRenderGL_2_1.h"
#include "DfxShortcutsDB.h"
#include "DfxToolbarDomain.h"
#include "DfxToolMap.h"
#include "DfxMaterialMime.h"

class DfxQSceneView: public QMainWindow {
	Q_OBJECT
private:

	DfxQSceneViewViewportWrangler *viewportWrangler;

	DfxQSceneViewport *viewport00;
	DfxQSceneViewport *viewport01;
	DfxQSceneViewport *viewport02;
	DfxQSceneViewport *viewport03;

	QTabWidget *menuTabs;

	QDockWidget *leftDock;

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
	void enterEvent(QEvent *event) override;

	void dragEnterEvent(QDragEnterEvent *event) override;

	void dropEvent(QDropEvent *event) override;

private:

	DfxToolbarDomain *buildToolbar();
public:

	DfxQSceneView();
	virtual ~DfxQSceneView();

public slots:
	void updateViewports();

	/// Viewport layouts
	void switchToSingle();
	void switchToQuad();
	void switchToDoubleVertical();
	void switchToDoubleHorizontal();
	void switchToTop2Bottom1();
	void switchToTop1Bottom2();
	void switchToTop3Bottom1();
	void switchToTop1Bottom3();
	void switchToLeft2Right1();
	void switchToLeft1Right2();
	void switchToLeft3Right1();
	void switchToLeft1Right3();
};
