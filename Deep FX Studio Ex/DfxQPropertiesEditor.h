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
#include <QSplitter>
#include <QScrollArea>
#include <QScrollbar>
#include "DfxSceneSelectionModel.h"
#include "DfxQMaterialNodeView.h"
#include "DfxQPropertiesHub.h"
#include "DfxBaseForm.h"
#include "DfxQSceneNodeForm.h"
#include "DfxQStyleScrollBar.h"

class DfxPropertiesEditor : public QMainWindow {
private:
	DfxBaseForm *_currentForm;
	QScrollArea *_scrollArea;
public:
	DfxPropertiesEditor(QWidget *parent = nullptr);
	~DfxPropertiesEditor();

	void clear();

public slots:
	void newObjectPropertiesForm(std::vector<deepsg::SgNode*> nodes);
};