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
#include <QAbstractItemView>
#include "DfxQFluentNodeEditor.h"
#include "DfxQComputePopupMenu.h"

DfxQComputePopupMenu::DfxQComputePopupMenu(QWidget *parent, DfxFluentNodeEditor *neditor) {
	_nodeEditor = neditor;

	_combobox = new QComboBox;
	_layout = new QVBoxLayout;

	_layout->addWidget(_combobox);

	setLayout(_layout);

	QObject::connect(_combobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxQComputePopupMenu::addNode);
}

DfxQComputePopupMenu::~DfxQComputePopupMenu() {
}

void DfxQComputePopupMenu::init() {
	QStringList list;
	list.push_back("");
	_combobox->addItem("");
	for (auto &desc : DfxComputeNodeDB::getInstance()->findAll()) {
		list.push_back(desc->name.c_str());
		_combobox->addItem(desc->name.c_str(), qVariantFromValue((void *)desc));
	}

	_completer = new QCompleter(list);
	_completer->setCaseSensitivity(Qt::CaseInsensitive);
	_combobox->setEditable(true);
	_combobox->setCompleter(_completer);
	_combobox->installEventFilter(this);
}

void DfxQComputePopupMenu::showMenuPopup() {
	popup(QCursor::pos());
	_combobox->setFocus();
	_combobox->setEditText("");
	_done = false;
}

void DfxQComputePopupMenu::addNode(int i) {
	
	if (isVisible() && i >= 0 && !_done) {
		auto qdata = _combobox->itemData(i);
		auto desc = (DfxComputeNodeDescription*)qdata.value<void*>();

		if (desc != nullptr) {
			auto space = DfxSpace::getInstance();

			auto compute_graph = space->findAbsolute(_nodeEditor->computeNodeCollection()->computeGraphPath());

			if (compute_graph != nullptr) {
				auto any = desc->instance(space);
				space->move(any, compute_graph);

				auto qnode = new DfxQComputeNode();
				qnode->buildNode(any);
				qnode->setDataObjectPath(any->absolutePath());

				_nodeEditor->computeNodeCollection()->addItem(qnode);
			}		
		}
		_done = true;
		hide();
	}
}

bool DfxQComputePopupMenu::eventFilter(QObject *obj, QEvent *event) {

	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		keyPressEvent(keyEvent);
	}
	
	return false;
}

void DfxQComputePopupMenu::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Return) {
		const int i = _combobox->findText(_combobox->currentText());

		if (i >= 1) {
			addNode(i);
		}
		else {
			hide();
		}
	}
}

