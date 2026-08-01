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

#include "ResourceQView.h"
#include "ResourceManager.h"
#include "ResourceQModel.h"
#include "ProjectManager.h"

ResourceQView::ResourceQView(QWidget *parent): QWidget(parent) {

	_splitter = new QSplitter(Qt::Horizontal);
	_treeView = new QTreeView();

	_propertiesLayout = new QVBoxLayout();
	_propertiesPanel = new ResourceQForm();

	_mainlayout = new QVBoxLayout();
	
	_contextMenu = new QMenu;
	_addResource = new QAction("Add Resource");
	_removeResource = new QAction("Remove Resource");
	
	_contextMenu->addAction(_addResource);
	_contextMenu->addAction(_removeResource);

	_propertiesLayout->setMargin(0);
	_propertiesPanel->setLayout(_propertiesLayout);

	_treeView->setModel(ResourceManager::getInstance()->model());

	_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

	_splitter->addWidget(_treeView);
	_splitter->addWidget(_propertiesPanel);
	_splitter->setSizes(QList<int> {700, 2000});

	_mainlayout->addWidget(_splitter);
	setLayout(_mainlayout);

	QObject::connect(_treeView, &QTreeView::customContextMenuRequested, this, &ResourceQView::resourceContextMenuSlot);
	QObject::connect(ProjectManager::getInstance(), &ProjectManager::projectChanged, this, &ResourceQView::expandAllTree);
	
	QObject::connect(_addResource, &QAction::triggered, this, &ResourceQView::addResourceSlot);

	QObject::connect(_treeView->selectionModel(), &QItemSelectionModel::selectionChanged, _propertiesPanel, &ResourceQForm::resourceChanged);
}

ResourceQView::~ResourceQView() {

}

void ResourceQView::resourceContextMenuSlot(const QPoint &pos) {
	QModelIndex index = _treeView->indexAt(pos);

	if (index.isValid()) {
		_contextMenu->exec(_treeView->viewport()->mapToGlobal(pos));
	}
}

void ResourceQView::addResourceSlot() {
	QStringList items;
	items << tr("Folder") << tr("File");

	bool ok00;
	QString item = QInputDialog::getItem(this, tr("Resource Type"), tr("Type:"), items, 0, false, &ok00);

	bool ok01;
	int count = QInputDialog::getInt(this, tr("Create How Many"), tr("count:"), 1, 1, 100, 1, &ok01);

	if (ok00 && ok01 && !item.isEmpty()) {
		auto index = _treeView->selectionModel()->currentIndex();

		if (index.isValid()) {
			auto rc = static_cast<Resource*>(index.internalPointer());

			if (rc != nullptr) {
				if (rc->type() != Resource::FILE) {
					for (int i = 0; i < count; i++) {
						auto new_rsc = new Resource;

						if (item == "Folder") {
							new_rsc->setName("Folder");
							new_rsc->setType(Resource::DIR);
						}
						else {
							new_rsc->setName("File");
							new_rsc->setType(Resource::FILE);
						}

						auto r_model = (ResourceQModel*)ResourceManager::getInstance()->model();
						r_model->beginAppendRowOp(rc);
						rc->addChild(new_rsc);
						r_model->endAppendRowOp();
					}

					_treeView->expand(index);
					_treeView->update();
				}
			}
		}
	}
}

void ResourceQView::expandAllTree() {
	_treeView->expandAll();
}