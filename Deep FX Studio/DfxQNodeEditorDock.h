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
#include <QDockWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QMenu>
#include "CompilationGlobals.h"
#include "DfxQNodeCommon.h"
#include "DfxSceneGraphMenuBar.h"

class DfxQNodeEditorDock : public QDockWidget {
	Q_OBJECT
private:
	DfxQNodeView *view;
	
	QMenu *fileMenu;
	QMenu *addMenu;
	QMenu *viewMenu;
	QMenu *layoutMenu;
		
	QAction *viewAllNodesAction;

	QAction *uniformLayoutAction;

	QBoxLayout *mainLayout;
	QBoxLayout *buttonLayout;
	QBoxLayout *navLayout;
	QWidget *wrapper;

	QPushButton *homeRootNode;
	QPushButton *previousRootNode;
	QLabel *levelNodeDescription;

	QPushButton *fileMenuButton;
	QPushButton *addMenuButton;
	QPushButton *viewMenuButton;
	QPushButton *layoutMenuButton;

public slots:
	void viewAllNodes();
	void uniformLayout();
	void dockTopLevelChange(bool);
	void updateLevelLabel(unsigned int);
public:
	DfxQNodeEditorDock(QWidget *parent = Q_NULLPTR);

	DfxQNodeView *getView();
	QMenu        *getAddMenu();
};
