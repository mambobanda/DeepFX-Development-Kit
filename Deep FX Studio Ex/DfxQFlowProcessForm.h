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
#include <DfxQNodeEditorCommon.h>
#include "DfxBaseForm.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"

class DfxQFlowProcessForm : public DfxBaseForm {
private:
	QPushButton *newProcess;
	QPushButton *removeProcess;
	QPushButton *moveProcessUp;
	QPushButton *moveProcessDown;
	QPushButton *renameProcess;

	QListWidget *processList;
	DfxQFlowProcessNode *_pnode;
	fl::Flow    *_flow;
	int			_fid;

	DfxFluentNodeEditor *_editor;
public:
	virtual ~DfxQFlowProcessForm();
	void create(DfxQFlowProcessNode *pnode, int fid, fl::Flow *fnode);
	void destroy() override;

	void clearList();
	void restoreList();

	DfxFluentNodeEditor *editor();
	void setEditor(DfxFluentNodeEditor *editor);

public slots:
	void addNewProcess();
	void moveExisitngProcessUp();
	void moveExisitngProcessDown();
	void removeSelectedProcess();
	void renameSelectedProcess();

	void nameChanged(const QString &text);

	void editComputeGraph(QListWidgetItem *item);
};