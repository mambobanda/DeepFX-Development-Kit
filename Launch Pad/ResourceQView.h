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


#include <QWidget>
#include <QLayout>
#include <QSplitter>
#include <QTreeView>
#include <QMenu> 
#include <QInputDialog>
#include "ResourceQForm.h"

class ResourceQView : public QWidget {
	Q_OBJECT
private:
	QBoxLayout *_mainlayout;

	QSplitter *_splitter;
	QTreeView *_treeView;

	QLayout *_propertiesLayout;
	ResourceQForm *_propertiesPanel;

	QMenu *_contextMenu;
	QAction *_addResource;
	QAction *_removeResource;

public:

	ResourceQView(QWidget *parent = nullptr);
	virtual ~ResourceQView();

public slots:
	void resourceContextMenuSlot(const QPoint &);
	void addResourceSlot();
	void expandAllTree();
};