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

#include "DfxSelectLayerDialog.h"

DfxSelectLayerDialog::DfxSelectLayerDialog(LayerSelectDialogCallback cb, DfxLayerItem *excludeitem):
	QDialog(Q_NULLPTR, Qt::WindowSystemMenuHint | Qt::WindowTitleHint ) {

	callback = cb;

	result = nullptr;

	boxLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
	buttonLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);
	list  = new QListView(this);
	model = new QStandardItemModel(this);
	ok = new QPushButton("Ok", this);
	cancel = new QPushButton("Cancel", this);

	DfxLayerItem *root = DfxLayerManager::getInstance()->getRoot();

	if (root != nullptr) {
		QStandardItem *parentItem = model->invisibleRootItem();

		for (size_t inx = 0; inx < root->childCount(); inx++) {

			DfxLayerItem *temp = root->getChild(inx);

			if (excludeitem != nullptr) {
				if (excludeitem->getID() == temp->getID()) continue;
			}

			QStandardItem *item1 = new QStandardItem(QPixmap(":/layers_tb/layer_icon").scaled(20, 20), QString(temp->getName().c_str()));
			QVariant v = qVariantFromValue((void*)temp);
			item1->setData(v);
			parentItem->appendRow(item1);
		}
	}

	list->setModel(model);

	buttonLayout->addWidget(ok);
	buttonLayout->addWidget(cancel);
	boxLayout->addWidget(list);
	boxLayout->addLayout(buttonLayout);

	setLayout(boxLayout);

	QObject::connect(ok, &QPushButton::clicked, [this]() {

		if (list->selectionModel()->selectedIndexes().size() > 0) {

			QStandardItem *item = model->itemFromIndex(list->selectionModel()->selectedIndexes()[0]);
			callback((DfxLayerItem*) item->data().value<void*>());
			close();
		}
	});

	QObject::connect(cancel, &QPushButton::clicked, [this]() {
		close();
	});

	
}