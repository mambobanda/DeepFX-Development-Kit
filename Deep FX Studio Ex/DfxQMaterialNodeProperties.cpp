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

#include "DfxQMaterialNodeProperties.h"

DfxQMaterialNodeProperties::DfxQMaterialNodeProperties(QWidget *parent):QMainWindow(parent) {
	_currentForm = nullptr;
	_scrollArea = new QScrollArea;
	mainLayout  = new QVBoxLayout();
	header		= new QLabel("Properties");
	mainWidget  = new QWidget;

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

DfxQMaterialNodeProperties::~DfxQMaterialNodeProperties() {

}

void DfxQMaterialNodeProperties::clear() {
	if (_currentForm != nullptr) {
		_currentForm->setParent(nullptr);
		delete _currentForm;
		_currentForm = nullptr;
	}
}

void DfxQMaterialNodeProperties::newNodePropertiesForm(DfxQMaterialNode *node) {
	clear();

	if (node != nullptr) {
		DfxQMaterialNodeForm *form = new DfxQMaterialNodeForm();
		if (form->create(node)) {
			_currentForm = form;
			_scrollArea->setWidget(form);
		}
		else {
			delete form;
		}
	}
}