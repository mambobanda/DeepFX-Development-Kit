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

#include <QObject>
#include <QLayout>
#include <QList>
#include "DfxQViewContainer.h"


class DfxQViewManager: public QObject {
	Q_OBJECT
private:
	static DfxQViewManager *_instance;
	DfxQViewContainer *_root;
	
public:
	DfxQViewManager();
	static DfxQViewManager *getInstance();

	DfxQViewContainer *root();
	void clear(DfxQViewContainer *container);

public slots:
	void splitViewContainer(Qt::Orientation dir, DfxQViewContainer *container, DfxQView *other, DfxQViewID vid, QSize ratio = QSize(1,1));
	void mergeViewContainerA(DfxQViewContainer *container);
	void mergeViewContainerB(DfxQViewContainer *container);
	void merge(DfxQView *view);
	void minMaxView(DfxQView *view);
};