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

#include <string>
#include <QtWidgets\QWidget>
#include <QtWidgets\QPushButton>
#include <QtWidgets\QMenu>
#include <QtWidgets\QAction>
#include <QtWidgets\QBoxLayout>
#include <QtWidgets\QLabel>

class DfxViewportMenus: public QWidget {
	Q_OBJECT
public:

	DfxViewportMenus(QWidget *parent = Q_NULLPTR);

	QBoxLayout *layout;

	QLabel *currentViewString;

	QPushButton *viewModeBtnMenu;
	QMenu *viewModeMenu;
	QAction *perspective;
	QAction *top;
	QAction *bottom;
	QAction *left;
	QAction *right;
	QAction *front;
	QAction *back;
	QAction *cameraFromView;
	QMenu *camera;

	QPushButton *renderModeBtnMenu;
	QMenu *renderModeMenu;
	QAction *solid;
	QAction *solidWire;
	QAction *wire;

	QPushButton *displayBtnMenu;
	QMenu *displayMenu;
	QAction *showHideGrid;
	QAction *showHideBackface;

	void setCurrentViewText(std::string str);
};