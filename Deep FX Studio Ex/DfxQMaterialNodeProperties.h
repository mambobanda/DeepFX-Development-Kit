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
#include "DfxSceneSelectionModel.h"
#include "DfxQMaterialNodeView.h"
#include "DfxQPropertiesHub.h"
#include "DfxBaseForm.h"
#include "DfxQMaterialNodeForm.h"
#include "DfxQStyleScrollBar.h"

class DfxQMaterialNodeProperties : public QMainWindow {
private:
	QBoxLayout *mainLayout;

	QLabel *header;

	QWidget *mainWidget;

	DfxBaseForm *_currentForm;
	QScrollArea *_scrollArea;
public:
	DfxQMaterialNodeProperties(QWidget *parent = nullptr);
	~DfxQMaterialNodeProperties();

	void clear();

public slots:
	void newNodePropertiesForm(DfxQMaterialNode *nodes);
};