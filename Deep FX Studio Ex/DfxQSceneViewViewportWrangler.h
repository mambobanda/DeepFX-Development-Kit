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

#include <QSplitter>
#include <QBoxLayout>
#include <QList>
#include "DfxQViewportCommon.h"

class DfxQSceneViewViewportWrangler : public QWidget {
private:

	QBoxLayout *blayout;

	DfxQSceneViewport *viewport00;
	DfxQSceneViewport *viewport01;
	DfxQSceneViewport *viewport02;
	DfxQSceneViewport *viewport03;

	QSplitter *a_split;
	QSplitter *b_split;
	QSplitter *c_split;
	QSplitter *d_split;

protected:
	bool eventFilter(QObject *obj, QEvent *event);
public:

	DfxQSceneViewViewportWrangler(QWidget *parent= Q_NULLPTR);
	virtual ~DfxQSceneViewViewportWrangler();

	void setup();

	void setViewport00(DfxQSceneViewport *);
	DfxQSceneViewport *getViewport00();
	
	void setViewport01(DfxQSceneViewport *);
	DfxQSceneViewport *getViewport01();

	void setViewport02(DfxQSceneViewport *);
	DfxQSceneViewport *getViewport02();

	void setViewport03(DfxQSceneViewport *);
	DfxQSceneViewport *getViewport03();

	void clear();

public slots:
	void singleLayout();
	void quadLayout();
	void doubleVerticalLayout();
	void doubleHorizontalLayout();
	void top2Bottom1Layout();
	void top1Bottom2Layout();
	void top3Bottom1Layout();
	void top1Bottom3Layout();
	void left2Right1Layout();
	void left1Right2Layout();
	void left3Right1Layout();
	void left1Right3Layout();
};