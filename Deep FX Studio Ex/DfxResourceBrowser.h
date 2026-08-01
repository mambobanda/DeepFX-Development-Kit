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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QWidget>
#include <QTreeView>
#include <QBoxLayout>
#include <DfxGlobal.h>
#include <DfxQStyleScrollBar.h>
#include "DfxResourceManager.h"


class DfxResourceBrowser : public QWidget {
	Q_OBJECT
private:
	QVBoxLayout *mainLayout;
	QTreeView *tree;

public:

	DfxResourceBrowser(QWidget *parent = Q_NULLPTR);
	virtual ~DfxResourceBrowser();

public slots:
	void autoExpandRoot(const QModelIndex &parent, int first, int last);

};