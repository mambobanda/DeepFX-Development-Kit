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
#include <QAction>
#include <QBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QLabel>

class DfxViewportOverlay: public QWidget{
	Q_OBJECT
public:
	QBoxLayout *topLayout;
	QBoxLayout *bottomLayout;
	QBoxLayout *leftTopLayout;
	QBoxLayout *rightTopLayout;
	QBoxLayout *layout;
	QBoxLayout *sceneEditLayout;

	QBoxLayout *viewModeVLayout;
	QBoxLayout *renderModeVLayout;
	QBoxLayout *displayModeVLayout;
	QBoxLayout *viewportVLayout;
	QBoxLayout *currentViewStringLayout;

	QBoxLayout *frameLayout;

	QBoxLayout *sceneStatsLayout;

	QLabel *currentViewString;

	QLabel *currentFrame;
	QLabel *triangleCountEst;

	QPushButton *translateBtn;
	QPushButton *rotateBtn;
	QPushButton *scaleBtn;
	QPushButton *objectModeBtn;
	QPushButton *vertexModeBtn;
	QPushButton *edgeModeBtn; 
	QPushButton *faceModeBtn;

	QPushButton *viewModeBtnMenu;
	QMenu *viewModeMenu;
	QAction *perspective;
	QAction *top;
	QAction *bottom;
	QAction *left;
	QAction *right;
	QAction *front;
	QAction *back;
	QMenu *cameraFromView;
	QMenu *camera;

	QPushButton *renderModeBtnMenu;
	QMenu *renderModeMenu;
	QAction *wireframe;
	QAction *wireSolidShade;
	QAction *solidShade;

	QPushButton *displayBtnMenu;
	QMenu *displayMenu;
	QAction *showHideGrid;
	QAction *showHideBackface;

	QPushButton *viewportLayoutBtnMenu;
	QMenu   *viewportLayoutMenu;
	QAction *single;
	QAction *quad;
	QAction *doubleVertical;
	QAction *doubleHorizontal;
	QAction *top2Bottom1;
	QAction *top1Bottom2;
	QAction *top3Bottom1;
	QAction *top1Bottom3;
	QAction *left2Right1;
	QAction *left1Right2;
	QAction *left3Right1;
	QAction *left1Right3;

	DfxViewportOverlay(QWidget *parent = nullptr);
	~DfxViewportOverlay();

	void setCurrentViewText(std::string str);
};
