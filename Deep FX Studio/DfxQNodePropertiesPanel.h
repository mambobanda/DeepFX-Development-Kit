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

#include <QGraphicsProxyWidget>
#include <QScrollArea>
#include <QGroupBox>
#include <QFormLayout>
#include <vector>
#include "DfxQControlBase.h"


class DfxQNodePropertiesPanel: public QGraphicsProxyWidget{
private:
	QScrollArea *area;
	QGroupBox *groupBox;
	QFormLayout *layout;

	std::vector<QWidget*> controls;
	std::vector<DfxQControlBase*> controlsBase;

	int fixedHeight;
	float pwidth;
	int dynaHeight;
public:
	enum { Type = QGraphicsItem::UserType + 4 };

	DfxQNodePropertiesPanel(QGraphicsItem *parent = 0);
	~DfxQNodePropertiesPanel();

	void setTitle(QString tit);

	void setHeight(int height);

	void addRow(QWidget *a, QWidget *b);

	void init();
};