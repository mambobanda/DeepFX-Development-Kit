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
#include <QListView>
#include <QMenu>
#include "DfxQMaterialNodeForm.h"
#include "DfxQMaterialNodeProperties.h"
#include "DfxQMaterialNodeView.h"
#include "DfxMaterialManager.h"
#include "DfxQMaterialNodeCollection.h"
#include "DfxProject.h"
#include "DfxNodeUtils.h"
#include "DfxQKeyBuffer.h"
#include "DfxShortcutsDB.h"

class DfxShaderEditor: public QMainWindow, public DfxQKeyBuffer {
private:
	QBoxLayout *materialViewLayout;
	QBoxLayout *nodeMenuLayout;
	QPushButton *addNodeMenuBtn;
	QPushButton *editNodeMenuBtn;
	QPushButton *viewMenuBtn;
	QPushButton *layoutMenuBtn;

	QMenu *addNodeMenu;
	QMenu *editNodeMenu;
	QMenu *viewNodeMenu;
	QMenu *layoutNodeMenu;

	QAction *editCopyAction;
	QAction *editCutAction;
	QAction *editPasteAction;

	QAction *viewViewAll;

	QLabel *materialsHeader;

	QBoxLayout *nodeViewLayout;
	QBoxLayout *materialListMenuLayout;
	QPushButton *newMaterial;
	QPushButton *deleteMaterial;

	QListView *materialsView;

	DfxQMaterialNodeProperties *propertiesView;

	DfxQMaterialNodeView *nodeView;

	QSplitter *mainSplitter;
	QSplitter *listViewPropertiesSplitter;

	QWidget *nodeViewWidget;
	QWidget *materialViewWidget;
	QWidget *propertiesViewWidget;

	// Lux Materials Menu
	QMenu *luxMenu;
	QMenu *luxMaterials;
	QMenu *luxTextures;
	QMenu *luxTextureUtility;

	// OSPRay 
	QMenu *ospMenu;
	QMenu *ospMaterials;
	QMenu *ospTextures;

private: 
	DfxQMaterialNodeCollection *nodeCollection;

	std::vector<std::pair<std::string, std::string>> _copyBuffer;
	std::vector<std::pair<std::string, std::string>> _cutBuffer;

private:
	void styleMenuButtons();
	size_t addNodeToCollection(fl::Any *);
	QAction *createAddNodeAction(QString actionLabel, std::string mat_name);
	void setupMenus();

protected:
	bool eventFilter(QObject *watched, QEvent *event) override;

public:
	DfxShaderEditor(QWidget *parent = nullptr);
	~DfxShaderEditor();

public slots:
	void matetrialViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void newMaterialInserted(const QModelIndex &parent, int first, int last);
	void nodesSelected(std::vector<DfxQMaterialBaseNode*> nodes);
	void resetSelectedMaterial();
	void removeSelectedMaterial();
	void cutSelectedNodes();
	void copySelectedNodes();
	void viewAll();
	void pasteNodes();
	void reset();
};