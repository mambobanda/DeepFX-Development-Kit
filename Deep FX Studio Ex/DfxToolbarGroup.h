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
#include <QLabel>
#include "DfxToolbarItem.h"
#include "DfxToolbarMenu.h"


class DfxToolbarGroup: public QWidget {
private:

	class GroupLabel : public QLabel {
	private:
	};

private:
	GroupLabel *label;
	QBoxLayout *layout;

public:

	DfxToolbarGroup(QString tit, QWidget *parent = nullptr);
	virtual ~DfxToolbarGroup();

	void addItem(DfxToolbarItem *);
	void addMenu(DfxToolbarMenu *);

};