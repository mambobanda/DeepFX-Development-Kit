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

#include "DfxLayerEditorWidget.h"


DfxLayerEditorWidget::DfxLayerEditorWidget(QWidget *parent):QWidget(parent) {

	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
	actionLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight , this);;

	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	edit = new QToolBar(this);
	edit->layout()->setMargin(0);
	edit->setIconSize(QSize(20,20));

	newLayer = new QAction(QIcon(":/layers_tb/add_layer"), "New Layer");
	addSelectionToLayer = new QAction(QIcon(":/layers_tb/add_to_layer"), "Move Selection Layer");
	duplicateLayer = new QAction(QIcon(":/layers_tb/duplicate_layer"), "Duplicate Layer");
	moveLayerUp = new QAction(QIcon(":/layers_tb/move_layer_up"), "Move Layer Up");
	moveLayerDown = new QAction(QIcon(":/layers_tb/move_layer_down"), "Move Layer Down");
	deleteLayer = new QAction(QIcon(":/layers_tb/remove_layer"), "Remove Layer");

	edit->addAction(newLayer);
	/*
	edit->addSeparator();
	edit->addAction(addSelectionToLayer);
	edit->addSeparator();
	edit->addAction(duplicateLayer);
	*/
	edit->addSeparator();
	edit->addAction(moveLayerUp);
	edit->addSeparator();
	edit->addAction(moveLayerDown);
	edit->addSeparator();
	edit->addAction(deleteLayer);
	
	layerTree = new QTreeView();
	layerTree->setModel(DfxLayerManager::getInstance()->getModel());
	layerTree->setColumnWidth(0, 140);
	layerTree->setColumnWidth(1, 40);
	layerTree->setColumnWidth(2, 40);
	layerTree->setColumnWidth(3, 40);
	layerTree->setSelectionBehavior(QTreeView::SelectRows);
	layerTree->setContextMenuPolicy(Qt::CustomContextMenu);

	selectFirstLayer();

	//mainLayout->addWidget(edit);
	mainLayout->addWidget(layerTree);

	contextMenuSelected = nullptr;
	setupContextMenus();

	//setLayout(mainLayout);

	QObject::connect(layerTree, &QTreeView::clicked, DfxLayerManager::getInstance(), &DfxLayerManager::updateState);
	QObject::connect(DfxLayerManager::getInstance(), &DfxLayerManager::removingLayerDone, this, &DfxLayerEditorWidget::resetLayerSelection);
	//QObject::connect(layerTree, &QTreeView::customContextMenuRequested, this, &DfxLayerEditorWidget::showNodeContextMenu);
	
	QObject::connect(newLayer, &QAction::triggered, this, &DfxLayerEditorWidget::addNewLayer);
	QObject::connect(deleteLayer, &QAction::triggered, this, &DfxLayerEditorWidget::removeLayer);
	QObject::connect(moveLayerDown, &QAction::triggered, this, &DfxLayerEditorWidget::moveLayerDownFunc);
	QObject::connect(moveLayerUp, &QAction::triggered, this, &DfxLayerEditorWidget::moveLayerUpFunc);
}

void DfxLayerEditorWidget::selectFirstLayer() {
	QItemSelection selection(layerTree->model()->index(0, 0), layerTree->model()->index(0, 3));
	layerTree->selectionModel()->select(selection, QItemSelectionModel::Select);
}

void DfxLayerEditorWidget::setupContextMenus() {

	nodeMenu = new QMenu("");
	layerMenu = new QMenu("");

	// Nodes Context Menus
	QAction *exportAction = new QAction("Export Node");
	QObject::connect(exportAction, &QAction::triggered, [this]() {
		if (contextMenuSelected != nullptr) {
			DfxSelectLayerDialog([this](DfxLayerItem *itm) {

				if (itm != nullptr) {
					DfxLayerManager::getInstance()
						->addImportNode(contextMenuSelected, itm);
				}
			}, 
			contextMenuSelected->getParent()).exec();
		}
	});

	QAction *deleteAction = new QAction("Delete Node");
	QObject::connect(deleteAction, &QAction::triggered, [this]() {
		if (contextMenuSelected != nullptr) {
			DfxLayerManager::getInstance()->removeNode(contextMenuSelected->getNode());
		}
	});

	nodeMenu->addAction(exportAction); 
	nodeMenu->addAction(deleteAction);

	// Layers Context Menu
	QAction *renameAction = new QAction("Rename");
	QObject::connect(renameAction, &QAction::triggered, [this]() {
		bool ok;
		QString text = QInputDialog::getText(this, tr("Rename Layer Dialog"), tr("New Name:"), QLineEdit::Normal, QString(), &ok);


		if (ok && !text.isEmpty()) {
			DfxLayerManager::getInstance()->renameLayer(text.toStdString(), contextMenuSelected);
		}
	
	});

	layerMenu->addAction(renameAction);
}

void DfxLayerEditorWidget::showNodeContextMenu(const QPoint &point) {

	QModelIndex selectedIndex = layerTree->indexAt(point);
	
	if (selectedIndex.isValid()) {

		DfxLayerItem *selected = static_cast<DfxLayerItem*>(selectedIndex.internalPointer());
		if (selected->getItemType() == DfxLayerItemType::NODE_ITEM) {
			contextMenuSelected = selected;
			nodeMenu->exec(layerTree->mapToGlobal(point));
		}
		else if (selected->getItemType() == DfxLayerItemType::LAYER_ITEM) {
			contextMenuSelected = selected;
			layerMenu->exec(layerTree->mapToGlobal(point));
		}
	}
}

void DfxLayerEditorWidget::addNewLayer() {
	DfxLayerManager::getInstance()->addNewLayer();

	if (!layerTree->selectionModel()->hasSelection()) {
		selectFirstLayer();
	}
}

void DfxLayerEditorWidget::removeLayer() {

	if (DfxLayerManager::getInstance()->layerCount() > 1) {
		DfxLayerManager::getInstance()->removeLayer();

		if (!layerTree->selectionModel()->hasSelection()) {
			selectFirstLayer();
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("Cannot remove last layer, at least one layer must exist.");
		msgBox.exec();
	}
}

void DfxLayerEditorWidget::moveLayerDownFunc() {
	DfxLayerManager::getInstance()->moveLayerDown();
}

void DfxLayerEditorWidget::moveLayerUpFunc() {
	DfxLayerManager::getInstance()->moveLayerUp();
}

void DfxLayerEditorWidget::resetLayerSelection() {
	//layerTree->setCurrentIndex(QModelIndex());
	layerTree->setCurrentIndex(layerTree->model()->index(0, 0));
}

void DfxLayerEditorWidget::updateTree() {
	layerTree->repaint();
}