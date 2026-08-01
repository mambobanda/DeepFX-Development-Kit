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
#include <QTreeWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include "DfxAttributeTree.h"
#include "DfxQStyleScrollBar.h"

class DfxQAttributeDialog: public QDialog {
private:
	QBoxLayout *mainLayout;
	QTreeWidget *_tree;

	std::string _attributePath;

	void populateTree();
	QTreeWidgetItem *addAttribute(DfxAttribute *);

	void styleTree();
	void styleButtons(QPushButton *btn);
public:

	DfxQAttributeDialog();
	~DfxQAttributeDialog();

	std::string attributePath();

public slots:
	void okBtnCkicked();
};