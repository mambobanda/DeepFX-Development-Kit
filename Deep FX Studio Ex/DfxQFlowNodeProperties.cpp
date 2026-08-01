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

#include "DfxQFlowNodeProperties.h"
#include "DfxQFluentEditorCrumb.h"

DfxQFlowNodeProperties::DfxQFlowNodeProperties(QWidget *parent) :QMainWindow(parent) {
	_currentForm = nullptr;
	_scrollArea = new QScrollArea;
	mainLayout = new QVBoxLayout();
	header = new QLabel("Properties");
	mainWidget = new QWidget;

	_scrollArea->horizontalScrollBar()->hide();
	_scrollArea->setWidgetResizable(true);

	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);

	mainLayout->addWidget(header);
	mainLayout->addWidget(_scrollArea);

	style::styleQScrollBar(_scrollArea->horizontalScrollBar());
	style::styleQScrollBar(_scrollArea->verticalScrollBar());

	header->setAlignment(Qt::AlignCenter);
	header->setStyleSheet(
		"QLabel { "
		"color: white;"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"padding-left: 0px;"
		"height: 18px;"
		"}");

	mainWidget->setLayout(mainLayout);
	setCentralWidget(mainWidget);
}

DfxQFlowNodeProperties::~DfxQFlowNodeProperties() {

}

void DfxQFlowNodeProperties::clear() {
	if (_currentForm != nullptr) {
		_currentForm->setParent(nullptr);
		delete _currentForm;
		_currentForm = nullptr;
	}
}

void DfxQFlowNodeProperties::showProcessProperties(DfxQFlowProcessNode *pnode, int fid, fl::Flow *fl) {
	clear();

	auto form = new DfxQFlowProcessForm();
	form->setEditor(_editor);
	form->create(pnode, fid, fl);
	_currentForm = form;

	_scrollArea->setWidget(form);
}

void DfxQFlowNodeProperties::showDecisionProperties(DfxQFlowDecisionNode *dnode, int fid, fl::Flow *fl) {
	clear();

	auto form = new DfxQFlowDecisionForm();
	form->setEditor(_editor);
	form->create(dnode, fid, fl);
	_currentForm = form;

	_scrollArea->setWidget(form);
}

void DfxQFlowNodeProperties::newNodePropertiesForm(DfxQMaterialNode *node) {
	clear();

	/*if (node != nullptr) {
		DfxQMarterialNodeForm *form = new DfxQMarterialNodeForm();
		if (form->create(node)) {
			_currentForm = form;
			_scrollArea->setWidget(form);
		}
		else {
			delete form;
		}
	}*/
}

DfxFluentNodeEditor *DfxQFlowNodeProperties::editor() {
	return _editor;
}

void DfxQFlowNodeProperties::setEditor(DfxFluentNodeEditor *edt) {
	_editor = edt;
}