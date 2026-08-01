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

#include <QWidget>
#include <QMenu>
#include <QBoxLayout>
#include <QTreeView>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QItemSelection>
#include <QToolbar>
#include "DfxQLayerModel.h"
#include "DfxLayerManager.h"
#include "DfxSelectLayerDialog.h"

class DfxLayerEditorWidget: public QWidget{
	Q_OBJECT
private:
	QToolBar *edit;

	QBoxLayout *mainLayout;
	QBoxLayout *actionLayout;

	QAction *newLayer;
	QAction *deleteLayer;
	QAction *duplicateLayer;
	QAction *addSelectionToLayer;
	QAction *moveLayerUp;
	QAction *moveLayerDown;

	QTreeView *layerTree;
private:
	QMenu *nodeMenu;
	QMenu *layerMenu;
	DfxLayerItem *contextMenuSelected;

	void setupContextMenus();
public:
	DfxLayerEditorWidget(QWidget *parent = Q_NULLPTR);
	void updateTree();
	void selectFirstLayer();
signals:
	void indexChanged();
public slots:
	void addNewLayer();
	void removeLayer();
	void showNodeContextMenu(const QPoint &point);
	void moveLayerDownFunc();
	void moveLayerUpFunc();
	void resetLayerSelection();
};