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

#include "DfxQPropertiesEditor.h"


DfxPropertiesEditor::DfxPropertiesEditor(QWidget *parent): QMainWindow(parent) {
	
	_currentForm = nullptr;
	_scrollArea = new QScrollArea;

	_scrollArea->horizontalScrollBar()->hide();
	_scrollArea->setWidgetResizable(true);

	style::styleQScrollBar(_scrollArea->horizontalScrollBar());
	style::styleQScrollBar(_scrollArea->verticalScrollBar());

	newObjectPropertiesForm(DfxSceneSelectionModel::getInstance()->selected());
	
	QObject::connect(DfxQPropertiesHub::getInstance(), &DfxQPropertiesHub::newSceneNodeSelected_signal,
		this, &DfxPropertiesEditor::newObjectPropertiesForm);

	setCentralWidget(_scrollArea);
}

DfxPropertiesEditor::~DfxPropertiesEditor() {

}

void DfxPropertiesEditor::clear() {

	if (_currentForm != nullptr) {
		_currentForm->setParent(nullptr);
		delete _currentForm;
		_currentForm = nullptr;
	}
}

void DfxPropertiesEditor::newObjectPropertiesForm(std::vector<deepsg::SgNode*> nodes) {
	clear();

	if (nodes.size()) {
		DfxSceneNodeForm *form = new DfxSceneNodeForm();
		form->create(nodes);
		_currentForm = form;
		_scrollArea->setWidget(form);
	}
}