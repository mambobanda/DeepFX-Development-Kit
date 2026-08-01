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

#include "DfxSceneGraphSelectionDialog.h"


DfxSceneGraphSelectionDialog::DfxSceneGraphSelectionDialog() {

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	QPushButton  *ok = new QPushButton("Ok");
	QPushButton  *cancel = new QPushButton("Cancel");

	_mode = 0;

	_referencePath = "";
	_node = nullptr;

	_layout = new QVBoxLayout(this);
	_tree = new QTreeView(this);

	_model = new DfxQDeepSGSelectionTreeModel();

	_layout->setContentsMargins(0, 0, 0, 0);

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	_tree->setModel(_model);

	_layout->addWidget(_tree);
	_layout->addWidget(buttonBox);

	_tree->setExpanded(_model->createExternalIndex(0, 0, deepsg::SgRootNode::getInstance()), true);

	setLayout(_layout);

	QObject::connect(_tree, &QTreeView::doubleClicked, this, &DfxSceneGraphSelectionDialog::setReferencePathAndCloseDialog);
	QObject::connect(ok, &QPushButton::clicked, this, &DfxSceneGraphSelectionDialog::accept);
	QObject::connect(cancel, &QPushButton::clicked, this, &DfxSceneGraphSelectionDialog::reject);
	QObject::connect(this, &DfxSceneGraphSelectionDialog::accepted, this, &DfxSceneGraphSelectionDialog::okBtnCkicked);

}

DfxSceneGraphSelectionDialog::~DfxSceneGraphSelectionDialog() {
	delete _model;
}

std::string DfxSceneGraphSelectionDialog::referencePath() {
	return _referencePath.toStdString();
}

void DfxSceneGraphSelectionDialog::setReferencePath(const QModelIndex &index) {

	if (!index.isValid()) {

		if ( _mode == 0 ) _referencePath = "";
		else _node = nullptr;

	}
	else {

		if ( _mode == 0 ) {
			deepsg::SgNode *item = static_cast<deepsg::SgNode*>(index.internalPointer());
			_referencePath = item->referencePath().c_str();
		}
		else {
			_node = static_cast<deepsg::SgNode*>(index.internalPointer());
		}

	}
}

void DfxSceneGraphSelectionDialog::setReferencePathAndCloseDialog(const QModelIndex &index) {
	accept();
}

void DfxSceneGraphSelectionDialog::okBtnCkicked() {
	setReferencePath(_tree->selectionModel()->currentIndex());
}

deepsg::SgNode *DfxSceneGraphSelectionDialog::node() {
	return _node;
}

int DfxSceneGraphSelectionDialog::mode() {
	return _mode;
}

void DfxSceneGraphSelectionDialog::setMode(int md) {
	_mode = md;
}