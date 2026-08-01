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
#include <QBoxLayout>
#include <QScrollArea>
#include <QScrollbar>
#include <QLabel>
#include <FlFlow.h>
#include "DfxSceneSelectionModel.h"
#include "DfxQMaterialNodeView.h"
#include "DfxQPropertiesHub.h"
#include "DfxBaseForm.h"
#include "DfxQStyleScrollBar.h"
#include "DfxQFlowProcessForm.h"
#include "DfxQFlowDecisionForm.h"
#include "DfxQNodeEditorCommon.h"

class DfxQFlowNodeProperties : public QMainWindow {
private:
	QBoxLayout *mainLayout;

	QLabel *header;

	QWidget *mainWidget;

	DfxBaseForm *_currentForm;
	QScrollArea *_scrollArea;

	DfxFluentNodeEditor *_editor;
public:

	DfxQFlowNodeProperties(QWidget *parent = nullptr);
	~DfxQFlowNodeProperties();

	void clear();

	void showProcessProperties(DfxQFlowProcessNode *pnode, int fid, fl::Flow *);
	void showDecisionProperties(DfxQFlowDecisionNode *dnode, int fid, fl::Flow *);

	DfxFluentNodeEditor *editor();
	void setEditor(DfxFluentNodeEditor *edt);

public slots:
	void newNodePropertiesForm(DfxQMaterialNode *nodes);
};