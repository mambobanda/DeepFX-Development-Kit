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
#include <QMenuBar>
#include <QMenu>
#include <QStyleFactory>
#include "DfxViewLayoutManager.h"
#include "DfxQLayoutWidget.h"
#include "DfxCommon.h"
#include "DfxQKeyBuffer.h"
#include "DfxQMainMenu.h"


class DfxQMainWindow: public QMainWindow, public DfxQKeyBuffer {
	Q_OBJECT
private:
	DfxQLayoutWidget *layoutWidget;

private:
	static DfxQMainWindow *_instance;
	static bool _isready;
protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void changeEvent(QEvent *event) override;

	DfxQMainWindow();
public:

	~DfxQMainWindow();

	static DfxQMainWindow *getInstance();
	static bool isReady();
public slots:
	void setWindowTitleInfo();

};