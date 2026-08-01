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

#include <FlFlow.h>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QinputDialog>
#include <QMessageBox>
#include "DfxBaseForm.h"
#include "DfxQNodeEditorCommon.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"

class DfxQFlowDecisionForm : public DfxBaseForm {
private:
	QPushButton *newProcess;
	QPushButton *removeProcess;

	QListWidget *processList;

	int _fid;
	fl::Flow *_flow;
	DfxQFlowDecisionNode *_dnode;

	DfxFluentNodeEditor *_editor;
public:
	virtual ~DfxQFlowDecisionForm();
	void create(DfxQFlowDecisionNode *dnode, int fid, fl::Flow *fnode);
	void destroy() override;

	DfxFluentNodeEditor *editor();
	void setEditor(DfxFluentNodeEditor *editor);

	void clearList();
	void restoreList();

public slots:
	void addNewProcess();
	void removeSelectedProcess();

	void nameChanged(const QString &text);

	void editComputeGraph(QListWidgetItem *item);
};