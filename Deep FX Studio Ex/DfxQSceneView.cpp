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

#include "DfxQSceneView.h"
#include "DfxSceneCleaner.h"
#include "DfxQMainWindow.h"
#include "DfxQSceneViewport.h"

DfxQSceneView::DfxQSceneView() {

	viewportWrangler = new DfxQSceneViewViewportWrangler(this);

	viewport00 = new DfxQSceneViewport(new DfxQViewportRenderGL_2_1);
	viewport01 = new DfxQSceneViewport(new DfxQViewportRenderGL_2_1);
	viewport02 = new DfxQSceneViewport(new DfxQViewportRenderGL_2_1);
	viewport03 = new DfxQSceneViewport(new DfxQViewportRenderGL_2_1);

	viewportWrangler->setViewport00(viewport00);
	viewportWrangler->setViewport01(viewport01);
	viewportWrangler->setViewport02(viewport02);
    viewportWrangler->setViewport03(viewport03);

	viewportWrangler->setup();

	leftDock = new QDockWidget();
	menuTabs = new QTabWidget();

	leftDock->setWidget(buildToolbar());
	leftDock->setFeatures(0);
	

	setCentralWidget(viewportWrangler);

	installEventFilter(this);

	setAcceptDrops(true);

	QObject::connect(DfxUpdater::getInstance(), &DfxUpdater::_sceneUpdated, this, &DfxQSceneView::updateViewports);

	QObject::connect(viewport00->getOverlay()->single, &QAction::triggered, this, &DfxQSceneView::switchToSingle);
	QObject::connect(viewport01->getOverlay()->single, &QAction::triggered, this, &DfxQSceneView::switchToSingle);
	QObject::connect(viewport02->getOverlay()->single, &QAction::triggered, this, &DfxQSceneView::switchToSingle);
	QObject::connect(viewport03->getOverlay()->single, &QAction::triggered, this, &DfxQSceneView::switchToSingle);

	QObject::connect(viewport00->getOverlay()->quad, &QAction::triggered, this, &DfxQSceneView::switchToQuad);
	QObject::connect(viewport01->getOverlay()->quad, &QAction::triggered, this, &DfxQSceneView::switchToQuad);
	QObject::connect(viewport02->getOverlay()->quad, &QAction::triggered, this, &DfxQSceneView::switchToQuad);
	QObject::connect(viewport03->getOverlay()->quad, &QAction::triggered, this, &DfxQSceneView::switchToQuad);

	QObject::connect(viewport00->getOverlay()->doubleVertical, &QAction::triggered, this, &DfxQSceneView::switchToDoubleVertical);
	QObject::connect(viewport01->getOverlay()->doubleVertical, &QAction::triggered, this, &DfxQSceneView::switchToDoubleVertical);
	QObject::connect(viewport02->getOverlay()->doubleVertical, &QAction::triggered, this, &DfxQSceneView::switchToDoubleVertical);
	QObject::connect(viewport03->getOverlay()->doubleVertical, &QAction::triggered, this, &DfxQSceneView::switchToDoubleVertical);

	QObject::connect(viewport00->getOverlay()->doubleHorizontal, &QAction::triggered, this, &DfxQSceneView::switchToDoubleHorizontal);
	QObject::connect(viewport01->getOverlay()->doubleHorizontal, &QAction::triggered, this, &DfxQSceneView::switchToDoubleHorizontal);
	QObject::connect(viewport02->getOverlay()->doubleHorizontal, &QAction::triggered, this, &DfxQSceneView::switchToDoubleHorizontal);
	QObject::connect(viewport03->getOverlay()->doubleHorizontal, &QAction::triggered, this, &DfxQSceneView::switchToDoubleHorizontal);

	QObject::connect(viewport00->getOverlay()->top2Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop2Bottom1);
	QObject::connect(viewport01->getOverlay()->top2Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop2Bottom1);
	QObject::connect(viewport02->getOverlay()->top2Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop2Bottom1);
	QObject::connect(viewport03->getOverlay()->top2Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop2Bottom1);

	QObject::connect(viewport00->getOverlay()->top1Bottom2, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom2);
	QObject::connect(viewport01->getOverlay()->top1Bottom2, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom2);
	QObject::connect(viewport02->getOverlay()->top1Bottom2, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom2);
	QObject::connect(viewport03->getOverlay()->top1Bottom2, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom2);

	QObject::connect(viewport00->getOverlay()->top3Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop3Bottom1);
	QObject::connect(viewport01->getOverlay()->top3Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop3Bottom1);
	QObject::connect(viewport02->getOverlay()->top3Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop3Bottom1);
	QObject::connect(viewport03->getOverlay()->top3Bottom1, &QAction::triggered, this, &DfxQSceneView::switchToTop3Bottom1);

	QObject::connect(viewport00->getOverlay()->top1Bottom3, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom3);
	QObject::connect(viewport01->getOverlay()->top1Bottom3, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom3);
	QObject::connect(viewport02->getOverlay()->top1Bottom3, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom3);
	QObject::connect(viewport03->getOverlay()->top1Bottom3, &QAction::triggered, this, &DfxQSceneView::switchToTop1Bottom3);

	QObject::connect(viewport00->getOverlay()->left2Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft2Right1);
	QObject::connect(viewport01->getOverlay()->left2Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft2Right1);
	QObject::connect(viewport02->getOverlay()->left2Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft2Right1);
	QObject::connect(viewport03->getOverlay()->left2Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft2Right1);
	
	QObject::connect(viewport00->getOverlay()->left1Right2, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right2);
	QObject::connect(viewport01->getOverlay()->left1Right2, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right2);
	QObject::connect(viewport02->getOverlay()->left1Right2, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right2);
	QObject::connect(viewport03->getOverlay()->left1Right2, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right2);

	QObject::connect(viewport00->getOverlay()->left3Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft3Right1);
	QObject::connect(viewport01->getOverlay()->left3Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft3Right1);
	QObject::connect(viewport02->getOverlay()->left3Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft3Right1);
	QObject::connect(viewport03->getOverlay()->left3Right1, &QAction::triggered, this, &DfxQSceneView::switchToLeft3Right1);

	QObject::connect(viewport00->getOverlay()->left1Right3, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right3);
	QObject::connect(viewport01->getOverlay()->left1Right3, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right3);
	QObject::connect(viewport02->getOverlay()->left1Right3, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right3);
	QObject::connect(viewport03->getOverlay()->left1Right3, &QAction::triggered, this, &DfxQSceneView::switchToLeft1Right3);

	QObject::connect(DfxSceneCleaner::getInstance(), &DfxSceneCleaner::resetSignal, this, &DfxQSceneView::switchToSingle);

}

DfxQSceneView::~DfxQSceneView() {

}

void DfxQSceneView::enterEvent(QEvent *event) {
	if (!hasFocus()) {
		setFocus();
	}
}

void DfxQSceneView::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasFormat("deepfx/material.bin")){
		event->acceptProposedAction();
	}
}

void DfxQSceneView::dropEvent(QDropEvent *event) {
	event->acceptProposedAction();
	auto mdata = qobject_cast <const DfxMimeDataBase*>(event->mimeData());

	if (mdata->type() == dfxmime::MATERIAL) {
		DfxGlobal::command::exec("linkMatToSurfs " + ((DfxMaterialMime*)mdata)->path());
	}
}

bool DfxQSceneView::eventFilter(QObject *obj, QEvent *event) {
	
	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (event->type() == QEvent::KeyPress) {

			if (DfxShortcutsDB::getInstance()->executeCommand("Scene View", kbuffer->keysToShortcut())) {
				return true;
			}
			else if (DfxShortcutsDB::getInstance()->executeCommand("Modelling", kbuffer->keysToShortcut())) {
				return true;
			}
			
		}
		
	}

	return QMainWindow::eventFilter(obj, event);
}


DfxToolbarDomain *DfxQSceneView::buildToolbar() {
	DfxToolbarDomain *dom;
	for (auto domain : DfxToolMap::getInstance()->domains()) {

		dom = new DfxToolbarDomain;

		for (auto section : domain->getChildren()) {
			
			DfxToolbarPage *page = new DfxToolbarPage;
			
			for (auto group : section->getChildren()) {

				DfxToolbarGroup *_group = new DfxToolbarGroup(group->getName().c_str());

				for (auto item : group->getChildren()) {
					
					DfxToolbarItem *titem = new DfxToolbarItem;
					titem->setText(item->getName().c_str());
					
					QObject::connect(titem, &DfxToolbarItem::clicked, [item]() {DfxGlobal::command::exec(item->getCommand()); });

					_group->addItem(titem);
				}

				page->addGroup(_group);
			}

			dom->addTab(page, section->getName().c_str());
		}
	}

	return dom;
}

void DfxQSceneView::updateViewports() {
	viewportWrangler->getViewport00()->updateView();
	viewportWrangler->getViewport01()->updateView();
	viewportWrangler->getViewport02()->updateView();
	viewportWrangler->getViewport03()->updateView();
}

void DfxQSceneView::switchToSingle() {
	viewportWrangler->singleLayout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToQuad() {
	viewportWrangler->quadLayout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToDoubleVertical() {
	viewportWrangler->doubleVerticalLayout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToDoubleHorizontal() {
	viewportWrangler->doubleHorizontalLayout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToTop2Bottom1() {
	viewportWrangler->top2Bottom1Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToTop1Bottom2() {
	viewportWrangler->top1Bottom2Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToTop3Bottom1() {
	viewportWrangler->top3Bottom1Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToTop1Bottom3() {
	viewportWrangler->top1Bottom3Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToLeft2Right1() {
	viewportWrangler->left2Right1Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToLeft1Right2() {
	viewportWrangler->left1Right2Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToLeft3Right1() {
	viewportWrangler->left3Right1Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

void DfxQSceneView::switchToLeft1Right3() {
	viewportWrangler->left1Right3Layout();

	QApplication::processEvents();
	DfxGlobal::command::exec("updateScene");
}

