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

#include <QPushButton>
#include <QBoxLayout>
#include <QMenu>
#include <QComboBox>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QMessageBox>
#include "DfxViewLayoutManager.h"

class DfxQLayoutWidget: public QWidget{
	Q_OBJECT
private:
	QHBoxLayout *widgetLayout;
	QComboBox *layout;
	QPushButton *options;
	
	QMenu *menu;
	QAction *save;
	QAction *remove;
	QAction *setAsDefault;

	void styleButton(QPushButton *qp);

private:
	bool _ignoreLayoutIndexChange;
public:

	DfxQLayoutWidget(QWidget *parent = Q_NULLPTR);
	virtual ~DfxQLayoutWidget();

public slots:
	void addAction();
	void removeAction();
	void setAsDefaultAction();
	void changeLayoutAction(int);
};