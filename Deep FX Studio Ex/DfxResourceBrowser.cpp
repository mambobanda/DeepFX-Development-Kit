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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxResourceBrowser.h"

DfxResourceBrowser::DfxResourceBrowser(QWidget *parent) :QWidget(parent) {

	mainLayout = new QVBoxLayout(this);
	tree = new QTreeView(this);

	mainLayout->setMargin(0);

	style::styleQScrollBar(tree->horizontalScrollBar());
	style::styleQScrollBar(tree->verticalScrollBar());

	tree->setModel(DfxResourceManager::getInstance()->model());

	//tree->setColumnWidth(0, 200);

	//tree->selectionModel()->deleteLater();
	//tree->setSelectionModel(DfxQSceneManager::getInstance()->selectionModel());

	tree->setExpanded(QModelIndex(), true);
	//tree->setDragEnabled(true);
	//tree->setDragDropMode(QAbstractItemView::InternalMove);
	//tree->setAcceptDrops(true);
	//tree->setDropIndicatorShown(true);
	tree->setAlternatingRowColors(true);
	//tree->setSelectionMode(QAbstractItemView::ExtendedSelection);

	mainLayout->addWidget(tree);

	tree->setExpanded(DfxResourceManager::getInstance()->model()->createExternalIndex(0, 0, DfxResourceManager::getInstance()->root()), true);

	//QObject::connect(DfxQSceneManager::getInstance()->model(), &DfxQDeepSGTreeModel::rowsInserted, this, &DfxResourceBrowser::autoExpandRoot);
	//QObject::connect(tree->se

	setLayout(mainLayout);
}

DfxResourceBrowser::~DfxResourceBrowser() {

}

void DfxResourceBrowser::autoExpandRoot(const QModelIndex &parent, int first, int last) {

	//deepsg::SgNode  *parentItem;

	//if (!parent.isValid())
	//	return;
	//else
	//	parentItem = static_cast<deepsg::SgNode*>(parent.internalPointer());

	//if (parentItem->getID() == deepsg::SgRootNode::getInstance()->getID()) {
	//	if (!tree->isExpanded(parent)) tree->setExpanded(parent, true);
	//}

}