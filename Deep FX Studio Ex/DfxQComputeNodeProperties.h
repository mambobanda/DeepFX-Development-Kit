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

#include <QMainWindow>
#include <QSplitter>
#include <QScrollArea>
#include <QScrollbar>
#include <QBoxLayout>
#include <QLabel>
#include "DfxSceneSelectionModel.h"
#include "DfxQComputeNodeView.h"
#include "DfxQPropertiesHub.h"
#include "DfxBaseForm.h"
#include "DfxQComputeNodeForm.h"
#include "DfxQStyleScrollBar.h"

class DfxQComputeNodeProperties: public QMainWindow {
private:
	QBoxLayout *mainLayout;

	QLabel *header;

	QWidget *mainWidget;

	DfxBaseForm *_currentForm;
	QScrollArea *_scrollArea;
public:
	DfxQComputeNodeProperties(QWidget *parent = nullptr);
	~DfxQComputeNodeProperties();

	void clear();

public slots:
	void newNodePropertiesForm(DfxQComputeBaseNode *nodes);
};