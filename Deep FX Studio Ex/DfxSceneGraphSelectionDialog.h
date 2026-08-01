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

#include <QDialog>
#include <QBoxLayout>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QPushButton>
#include "DfxQDeepSGSelectionTreeModel.h"
#include "DfxQSceneManager.h"

class DfxSceneGraphSelectionDialog: public QDialog {
private:
	QBoxLayout *_layout;
	QTreeView  *_tree;

	DfxQDeepSGSelectionTreeModel *_model;

	QString _referencePath;
	deepsg::SgNode *_node;

	int _mode;
public:

	enum { PATH = 0, NODE = 1, };

	DfxSceneGraphSelectionDialog();
	virtual ~DfxSceneGraphSelectionDialog();

	std::string referencePath();
	deepsg::SgNode *node();

	int mode();
	void setMode(int );

public slots:
	void setReferencePath(const QModelIndex &index);
	void setReferencePathAndCloseDialog(const QModelIndex &index);
	void okBtnCkicked();
};