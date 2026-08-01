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


#include "DfxViewportOverlay.h"

DfxViewportOverlay::DfxViewportOverlay(QWidget *parent):QWidget(parent) {

	int icon_btn_sz = 28;
	int icon_sz     = 18;

	layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->setSizeConstraint(QLayout::SetFixedSize);

	leftTopLayout = new QHBoxLayout();
	leftTopLayout->setAlignment(Qt::AlignTop);
	leftTopLayout->setMargin(0);
	leftTopLayout->setSpacing(3);

	rightTopLayout = new QHBoxLayout();
	rightTopLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
	rightTopLayout->setMargin(0);
	rightTopLayout->setSpacing(0);

	topLayout = new QHBoxLayout;
	topLayout->setAlignment(Qt::AlignTop);
	topLayout->setMargin(0);
	topLayout->setSpacing(0);

	bottomLayout = new QVBoxLayout;
	bottomLayout->setAlignment(Qt::AlignBottom);
	bottomLayout->setMargin(0);
	bottomLayout->setSpacing(0);

	sceneEditLayout = new QVBoxLayout();
	sceneEditLayout->setMargin(0);

	viewModeVLayout = new QVBoxLayout();
	renderModeVLayout = new QVBoxLayout();
	viewportVLayout = new QVBoxLayout();
	currentViewStringLayout = new QVBoxLayout();

	frameLayout = new QVBoxLayout();
	frameLayout->setMargin(0);

	currentFrame = new QLabel("Frame");
	triangleCountEst = new QLabel("Polygon Count");

	//displayModeVLayout = new QVBoxLayout(this);

	currentViewStringLayout->setMargin(5);

	currentViewString = new QLabel("");
	currentViewString->setStyleSheet("QWidget{background-color: rgba(0,0,0,0);}");

	translateBtn	= new QPushButton(QPixmap(":/viewport_tb/translate"), "", this);
	rotateBtn		= new QPushButton(QPixmap(":/viewport_tb/rotate"), "", this);
	scaleBtn		= new QPushButton(QPixmap(":/viewport_tb/scale"), "", this);
	objectModeBtn	= new QPushButton(QPixmap(":/viewport_tb/object_mode"), "", this);
	vertexModeBtn	= new QPushButton(QPixmap(":/viewport_tb/vertex_mode"), "", this);
	edgeModeBtn		= new QPushButton(QPixmap(":/viewport_tb/edge_mode"), "", this);
	faceModeBtn		= new QPushButton(QPixmap(":/viewport_tb/face_mode"), "", this);

	translateBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	translateBtn->setIconSize(QSize(icon_sz, icon_sz));

	rotateBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	rotateBtn->setIconSize(QSize(icon_sz, icon_sz));

	scaleBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	scaleBtn->setIconSize(QSize(icon_sz, icon_sz));

	objectModeBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	objectModeBtn->setIconSize(QSize(icon_sz, icon_sz));

	vertexModeBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	vertexModeBtn->setIconSize(QSize(icon_sz, icon_sz));

	edgeModeBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	edgeModeBtn->setIconSize(QSize(icon_sz, icon_sz));

	faceModeBtn->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	faceModeBtn->setIconSize(QSize(icon_sz, icon_sz));

	viewModeBtnMenu		= new QPushButton("View", this);
	viewModeMenu		= new QMenu("View");
	perspective			= new QAction("Perspective");
	top					= new QAction("Top");
	bottom				= new QAction("Bottom");
	left				= new QAction("Left");
	right				= new QAction("Right");
	front				= new QAction("Front");
	back				= new QAction("Back");
	cameraFromView		= new QMenu("Align Camera");
	camera				= new QMenu("Camera");

	viewModeMenu->addAction(perspective);
	viewModeMenu->addAction(top);
	viewModeMenu->addAction(bottom);
	viewModeMenu->addAction(left);
	viewModeMenu->addAction(right);
	viewModeMenu->addAction(front);
	viewModeMenu->addAction(back);

	viewModeMenu->addMenu(cameraFromView);
	viewModeMenu->addMenu(camera);

	viewModeBtnMenu->setMenu(viewModeMenu);

	renderModeBtnMenu		= new QPushButton("Render Mode", this);
	renderModeMenu			= new QMenu("Render Mode");
	wireframe				= new QAction("Wireframe");
	wireSolidShade			= new QAction("Wireframe Solid Shade");
	solidShade				= new QAction("Solid Shade");

	renderModeMenu->addAction(wireframe);
	renderModeMenu->addAction(wireSolidShade);
	renderModeMenu->addAction(solidShade);

	renderModeBtnMenu->setMenu(renderModeMenu);

	//displayBtnMenu = new QPushButton("Display", this);
	displayMenu = new QMenu("Display");
	showHideGrid = new QAction("Show/Hide Grid");
	showHideBackface = new QAction("Show/Hide Backface");

	displayMenu->addAction(showHideGrid);
	//displayMenu->addAction(showHideBackface);
	//displayBtnMenu->setMenu(displayMenu);

	viewportLayoutBtnMenu = new QPushButton(QPixmap(":/icon/view_menu"), "", this);
	viewportLayoutMenu	  = new QMenu("");
	single			= new QAction(QPixmap(":/icon/single_lt"), "Single");
	quad			= new QAction(QPixmap(":/icon/quad_lt"), "Quad");
	doubleVertical	= new QAction(QPixmap(":/icon/double_vertical_lt"), "Double Vertical");
	doubleHorizontal  = new QAction(QPixmap(":/icon/double_horizontal_lt"), "Double Horizontal");
	top2Bottom1		= new QAction(QPixmap(":/icon/top2_bottom1_lt"), "2 Top / 1 Bottom");
	top1Bottom2		= new QAction(QPixmap(":/icon/top1_bottom2_lt"), "1 Top / 2 Bottom");
	top3Bottom1		= new QAction(QPixmap(":/icon/top3_bottom1_lt"), "3 Top / 1 Bottom");;
	top1Bottom3		= new QAction(QPixmap(":/icon/top1_bottom3_lt"), "1 Top / 3 Bottom");;
	left2Right1		= new QAction(QPixmap(":/icon/left2_right1_lt"), "2 Left / 1Right");;
	left1Right2		= new QAction(QPixmap(":/icon/left1_right2_lt"), "1 Left / 2 Right");;
	left3Right1		= new QAction(QPixmap(":/icon/left3_right1_lt"), "3 Left / 1 Right");;
	left1Right3		= new QAction(QPixmap(":/icon/left1_right3_lt"), "1 Left / 3 Right");;

	viewportLayoutBtnMenu->setFixedSize(QSize(21, 21));
	viewportLayoutBtnMenu->setIconSize(QSize(11, 11));

	viewportLayoutMenu->addAction(single);
	viewportLayoutMenu->addAction(quad);
	viewportLayoutMenu->addAction(doubleVertical);
	viewportLayoutMenu->addAction(doubleHorizontal);
	viewportLayoutMenu->addAction(top2Bottom1);
	viewportLayoutMenu->addAction(top1Bottom2);
	viewportLayoutMenu->addAction(top3Bottom1);
	viewportLayoutMenu->addAction(top1Bottom3);
	viewportLayoutMenu->addAction(left2Right1);
	viewportLayoutMenu->addAction(left1Right2);
	viewportLayoutMenu->addAction(left3Right1);
	viewportLayoutMenu->addAction(left1Right3);

	viewportLayoutBtnMenu->setMenu(viewportLayoutMenu);

	sceneEditLayout->addWidget(translateBtn);
	sceneEditLayout->addWidget(rotateBtn);
	sceneEditLayout->addWidget(scaleBtn);
	sceneEditLayout->addWidget(objectModeBtn);
	sceneEditLayout->addWidget(vertexModeBtn);
	sceneEditLayout->addWidget(edgeModeBtn);
	sceneEditLayout->addWidget(faceModeBtn);

	viewModeVLayout->addWidget(viewModeBtnMenu);
	viewModeVLayout->addStretch();

	renderModeVLayout->addWidget(renderModeBtnMenu);
	renderModeVLayout->addStretch();

	renderModeVLayout->addWidget(renderModeBtnMenu);
	renderModeVLayout->addStretch();

	viewportVLayout->addWidget(viewportLayoutBtnMenu);
	viewportVLayout->addStretch();

	currentViewStringLayout->addWidget(currentViewString);
	currentViewStringLayout->addStretch();

	/*frameLayout->addWidget(currentFrame);
	frameLayout->addWidget(new QPushButton("Button"));
	frameLayout->addWidget(triangleCountEst);*/
	//frameLayout->addStretch();

	layout->addLayout(topLayout);
	layout->addLayout(bottomLayout);

	topLayout->addLayout(leftTopLayout);
	topLayout->addLayout(rightTopLayout);

	leftTopLayout->addLayout(sceneEditLayout);
	leftTopLayout->addLayout(viewModeVLayout);
	leftTopLayout->addLayout(renderModeVLayout);
	leftTopLayout->addLayout(viewportVLayout);
	//layout->addLayout(displayModeVLayout);
	leftTopLayout->addLayout(currentViewStringLayout);

	//rightTopLayout->addLayout(frameLayout);

	setLayout(layout);
	move(10l, 10);
}

DfxViewportOverlay::~DfxViewportOverlay() {

}

void DfxViewportOverlay::setCurrentViewText(std::string str) {
	currentViewString->setText(str.c_str());
}
