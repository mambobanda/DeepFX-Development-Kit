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

#include <QHeaderView>
#include "DfxSceneGraphEditor.h"
#include "DfxQMainWindow.h"

DfxSceneGraphEditor::DfxSceneGraphEditor(QWidget *parent):QWidget(parent) {
	
	mainLayout = new QVBoxLayout(this);
	tree	   = new QTreeView(this);

	mainLayout->setMargin(0);

	style::styleQScrollBar(tree->horizontalScrollBar());
	style::styleQScrollBar(tree->verticalScrollBar());

	tree->setModel(DfxQSceneManager::getInstance()->model());
	
	// tree->setColumnWidth(0, 200);

	tree->setUniformRowHeights(true);
	tree->selectionModel()->deleteLater();
	tree->setSelectionModel(DfxQSceneManager::getInstance()->selectionModel());

	tree->setExpanded(QModelIndex(), true);
	tree->setDragEnabled(true);
	tree->setDragDropMode(QAbstractItemView::InternalMove);
	tree->setAcceptDrops(true);
	tree->setDropIndicatorShown(true);
	tree->setAlternatingRowColors(true);
	tree->setSelectionMode(QAbstractItemView::ExtendedSelection);

	resizeHeader();

	mainLayout->addWidget(tree);

	tree->setExpanded(DfxQSceneManager::getInstance()->model()->createExternalIndex(0,0, deepsg::SgRootNode::getInstance()), true);
	
	installEventFilter(this);

	QObject::connect(DfxQSceneManager::getInstance()->model(), &DfxQDeepSGTreeModel::rowsInserted, this, &DfxSceneGraphEditor::autoExpandRoot);
	QObject::connect(DfxQSceneManager::getInstance()->model(), &DfxQDeepSGTreeModel::rowsMoved, this, &DfxSceneGraphEditor::resizeHeader);

	QObject::connect(tree, &QTreeView::clicked, this, &DfxSceneGraphEditor::toggleVisiblity);
	QObject::connect(tree, &QTreeView::clicked, this, &DfxSceneGraphEditor::toggleRenderable);

	setLayout(mainLayout);
}

DfxSceneGraphEditor::~DfxSceneGraphEditor() {

}

void DfxSceneGraphEditor::autoExpandRoot(const QModelIndex &parent, int first, int last) {

	deepsg::SgNode  *parentItem;

	if (!parent.isValid())
		return;
	else
		parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());

	if (parentItem->getID() == deepsg::SgRootNode::getInstance()->getID()) {
		if (!tree->isExpanded(parent)) tree->setExpanded(parent, true);
	}

}

void DfxSceneGraphEditor::resizeHeader() {
	tree->header()->setSectionsMovable(false);
	tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	tree->header()->setSectionResizeMode(1, QHeaderView::Fixed);
	tree->header()->setSectionResizeMode(2, QHeaderView::Fixed);
	tree->header()->setStretchLastSection(false);

	tree->header()->resizeSection(1, 5);
	tree->header()->resizeSection(2, 5);
}

void DfxSceneGraphEditor::toggleVisiblity(const QModelIndex &itemindex) {
	if (itemindex.isValid()) {
		if (itemindex.column() == 1) {
			auto node = static_cast<deepsg::SgNode*>(itemindex.internalPointer());
			DfxQSceneManager::getInstance()->toggleVisibilty(node);
		}
	}
}

void DfxSceneGraphEditor::toggleRenderable(const QModelIndex &itemindex) {
	if (itemindex.isValid()) {
		if (itemindex.column() == 2) {
			auto node = static_cast<deepsg::SgNode*>(itemindex.internalPointer());
			DfxQSceneManager::getInstance()->toggleRenderable(node);
		}
	}
}

bool DfxSceneGraphEditor::eventFilter(QObject *watched, QEvent *event) {
	
	if (DfxQMainWindow::isReady()) {
		auto kbuffer = (DfxQKeyBuffer*)DfxQMainWindow::getInstance();

		if (event->type() == QEvent::KeyPress) {

			if (DfxShortcutsDB::getInstance()->executeCommand("Outliner", kbuffer->keysToShortcut())) {
				return true;
			}
		}
	}

	return QWidget::eventFilter(watched, event);
}