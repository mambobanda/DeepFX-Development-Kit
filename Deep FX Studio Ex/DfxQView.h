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
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QBoxLayout>
#include <QMenu>
#include "DfxQCommonUI.h"

class DfxQView : public QWidget {
	Q_OBJECT
private:

	class Tab : public QFrame {
	public:
		QLabel *title;
		QBoxLayout *layout;

		QPushButton *menu;
		QPushButton *minmax;
		QPushButton *close;

		Tab(QWidget *parent = Q_NULLPTR);
		~Tab();

		void setTitle(QString ttl);
	};

private:
	DfxQViewContainer *container;

	QBoxLayout *mainLayout;

	QBoxLayout *headerLayout; 
	QBoxLayout *headerLayoutLeft;
	QBoxLayout *headerLayoutRight;

	Tab *viewTab;

	QWidget *centralWidget;

	bool maximised;

private:
	
	void constructViewMenu(Tab *tab);
public:

	DfxQView();
	virtual ~DfxQView();

	DfxQViewContainer *getContainer();
	void setContainer(DfxQViewContainer *);
	void clearContainer();

	QWidget *getCentralWidget();
	void setCentralWidget(QWidget *);
	void removeCentralWidget();

	QPushButton *getViewMenuButton();
	QPushButton *getViewMinMaxButton();
	QPushButton *getViewCloseButton();

	bool isViewMaximised();
	void setViewIsMaximised(bool);

	void setTitle(QString str);

public slots:
	void closeView(bool);
	void minMaxView(bool);
};