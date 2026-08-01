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
#include <QBoxLayout>
#include <QSplitter>
#include <QList>
#include <DfxQView.h>
#include <DfxQCommonUI.h>
#include "DfxQViewID.h"

class DfxQViewContainer : public QWidget{
	Q_OBJECT
private:

	bool _root;

	DfxQViewContainer *parentContainer;

	QBoxLayout *blayout;

	QSplitter *splitter;
	DfxQView *view;
	DfxQViewID viewid;
	DfxQViewContainer *A;
	DfxQViewContainer *B;

public:
	DfxQViewContainer(QWidget *parent = Q_NULLPTR);
	~DfxQViewContainer();

	void setRoot(bool);
	bool isRoot();

	DfxQView *getView();
	DfxQViewID getViewID();

	void setViewID(DfxQViewID vid);
	void setView(DfxQView *wgt);
	void removeView(DfxQView *wgt);

	DfxQViewContainer *getA();
	DfxQViewContainer *getB();

	void viewToNull();

	void deleteA();
	void deleteB();
	void deleteView();

	void setA(DfxQViewContainer *);
	void setB(DfxQViewContainer *);

	void deleteSplitter();
	bool isSplitterNull();
	void newSplitter(Qt::Orientation dir, DfxQViewContainer *a, DfxQViewContainer *b, QSize ratio = QSize(1,1));
	QSplitter *getSplitter();

	DfxQViewContainer *getParentContainer();
	void setParentContainer(DfxQViewContainer *);
};