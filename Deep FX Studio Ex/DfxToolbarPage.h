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

#include <QTabWidget>
#include <QScrollArea>
#include "DfxToolbarGroup.h"


class DfxToolbarPage: public QScrollArea {
private:
	QBoxLayout *layout;
	QWidget *pageWidget;
public:
	
	DfxToolbarPage(QWidget *parent=nullptr);
	virtual ~DfxToolbarPage();

	void addGroup(DfxToolbarGroup *);
};