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

#include "DfxViewportToolbar.h"

DfxViewportToolbar::DfxViewportToolbar() {

	_bar = new QToolBar(DfxCoreApplication::getInstance());
	_bar->setMovable(false);

	translateAction = new QAction(QIcon(":/viewport_tb/translate"), "Translate");
	rotateAction = new QAction(QIcon(":/viewport_tb/rotate"), "Rotate");
	scaleAction = new QAction(QIcon(":/viewport_tb/scale"), "Scale");

	planePrimitiveAction = new QAction(QIcon(":/viewport_tb/plane"), "Create Plane");
	boxPrimitiveAction = new QAction(QIcon(":/viewport_tb/box"), "Create Box");
	spherePrimitiveAction = new QAction(QIcon(":/viewport_tb/sphere"), "Create Sphere");
	torusPrimitiveAction = new QAction(QIcon(":/viewport_tb/torus"), "Create Torus");

	_bar->addAction(translateAction);
	_bar->addAction(rotateAction);
	_bar->addAction(scaleAction);
	_bar->addSeparator();
	/*_bar->addAction(planePrimitiveAction);
	_bar->addAction(boxPrimitiveAction);
	_bar->addAction(spherePrimitiveAction);
	_bar->addAction(torusPrimitiveAction);*/

	lastTransformAction = nullptr;

	QObject::connect(translateAction, &QAction::triggered, this, &DfxViewportToolbar::translateSlot);
	QObject::connect(scaleAction, &QAction::triggered, this, &DfxViewportToolbar::scaleSlot);
	QObject::connect(rotateAction, &QAction::triggered, this, &DfxViewportToolbar::rotateSlot);

	/*QObject::connect(planePrimitiveAction, &QAction::triggered, this, &DfxViewportToolbar::planeSlot);
	QObject::connect(boxPrimitiveAction, &QAction::triggered, this, &DfxViewportToolbar::boxSlot);
	QObject::connect(spherePrimitiveAction, &QAction::triggered, this, &DfxViewportToolbar::sphereSlot);
	QObject::connect(torusPrimitiveAction, &QAction::triggered, this, &DfxViewportToolbar::torusSlot);*/
}

QToolBar *DfxViewportToolbar::getToolBar() {
	return _bar;
}

void DfxViewportToolbar::addToolbar() {
	DfxViewportToolbar *tb = new DfxViewportToolbar();
	DfxCoreApplication::getInstance()->addToolBar(Qt::LeftToolBarArea, tb->getToolBar());	
}

void DfxViewportToolbar::translateSlot() {
	DfxScene::getInstance()->setTransformMode(DfxScene::TRANSLATE_TRANSFORM);
	DfxGlobal::command::exec("clearSelection");
	DfxGlobal::command::exec("updateViewport");
}

void DfxViewportToolbar::scaleSlot() {
	DfxScene::getInstance()->setTransformMode(DfxScene::SCALE_TRANSFORM);
	DfxGlobal::command::exec("clearSelection");
	DfxGlobal::command::exec("updateViewport");
}

void DfxViewportToolbar::rotateSlot() {
	DfxScene::getInstance()->setTransformMode(DfxScene::ROTATE_TRANSFORM);
	DfxGlobal::command::exec("clearSelection");
	DfxGlobal::command::exec("updateViewport");
}

void DfxViewportToolbar::planeSlot() {
	DfxGlobal::command::exec("createPlane");
}

void DfxViewportToolbar::boxSlot() {
	DfxGlobal::command::exec("createBox");
}

void DfxViewportToolbar::sphereSlot() {
	DfxGlobal::command::exec("createSphere");
}

void DfxViewportToolbar::torusSlot() {
	DfxGlobal::command::exec("createTorus");
}