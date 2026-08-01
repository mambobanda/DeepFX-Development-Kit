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

#include "DfxViewportMenus.h"


DfxViewportMenus::DfxViewportMenus(QWidget *parent): QWidget(parent) {

	layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

	currentViewString = new QLabel("");

	viewModeBtnMenu = new QPushButton("View", this);
	viewModeMenu	= new QMenu("View", this);
	perspective		= new QAction("Perspective");
	top				= new QAction("Top");
	bottom			= new QAction("Bottom");
	left			= new QAction("Left");
	right			= new QAction("Right");
	front			= new QAction("Front");
	back			= new QAction("Back");
	cameraFromView  = new QAction("Camera From View");
	camera			= new QMenu("Camera");

	viewModeMenu->addAction(perspective);
	viewModeMenu->addAction(top);
	viewModeMenu->addAction(bottom);
	viewModeMenu->addAction(left);
	viewModeMenu->addAction(right);
	viewModeMenu->addAction(front);
	viewModeMenu->addAction(back);

//#if ENABLE_PROTOTYPE_FEATURES
	viewModeMenu->addAction(cameraFromView);
	viewModeMenu->addMenu(camera);
//#endif

	viewModeBtnMenu->setMenu(viewModeMenu);

	renderModeBtnMenu = new QPushButton("Render Mode", this);
	renderModeMenu	  = new QMenu("Render Mode");
	solid			  = new QAction("Solid");
	solidWire		  = new QAction("Solid Wire");
	wire			  = new QAction("Wireframe");

	renderModeMenu->addAction(solid);
	renderModeMenu->addAction(wire);

	renderModeBtnMenu->setMenu(renderModeMenu);
	
	displayBtnMenu	= new QPushButton("Display", this);
	displayMenu		= new QMenu("Display");
	showHideGrid	= new QAction("Show/Hide Grid");
	showHideBackface = new QAction("Show/Hide Backface");

	displayMenu->addAction(showHideGrid);
	//displayMenu->addAction(showHideBackface);

	displayBtnMenu->setMenu(displayMenu);

	layout->addWidget(viewModeBtnMenu);
	layout->addWidget(renderModeBtnMenu);
	layout->addWidget(displayBtnMenu);
	layout->addWidget(currentViewString);

	setLayout(layout);
	move(10l, 10);
}


void DfxViewportMenus::setCurrentViewText(std::string str) {
	currentViewString->setText(str.c_str());
}