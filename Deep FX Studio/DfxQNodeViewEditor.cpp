/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxQNodeViewEditor.h"


DfxQNodeViewEditor::DfxQNodeViewEditor(QWidget *parent): QWidget(parent) {

	QMenu *_addMenu = new QMenu("Add", this);

	QMenu *mathMenu = new QMenu("Math", this);
	QMenu *mathScalarMenu = new QMenu("Scalar", this);

	_addMenu->addMenu(mathMenu);
	mathMenu->addMenu(mathScalarMenu);

	QAction *addScalerAddNode = new QAction("Add", this);
	QAction *addScalerSubtractNode = new QAction("Subtract", this);
	QAction *addScalerMultiplyNode = new QAction("Multiply", this);
	QAction *addScalerDivideNode = new QAction("", this);

	mathScalarMenu->addAction(addScalerAddNode);
	mathScalarMenu->addAction(addScalerSubtractNode);
	mathScalarMenu->addAction(addScalerMultiplyNode);
	mathScalarMenu->addAction(addScalerDivideNode);

	view = new DfxQNodeView();
	menubar = new DfxSceneGraphMenuBar();
	QBoxLayout *menulayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	vlayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

	menulayout->addWidget(_addMenu);

	vlayout->addLayout(menulayout);
	vlayout->addWidget(view);

	setLayout(menulayout);
}