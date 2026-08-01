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

#include "DfxQBoolControl.h"

DfxQBoolControl::DfxQBoolControl(std::string flabel, QWidget *parent) :DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_combobox = new QComboBox(this);

	_layout->setMargin(0);

	styleComboBox(_combobox);

	std::string label = flabel;

	if (label.length() > 0) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_combobox);

	setLayout(_layout);
}

DfxQBoolControl::DfxQBoolControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent) {
	_layout = new QHBoxLayout();
	_label = nullptr;
	_combobox = new QComboBox(this);

	_layout->setMargin(0);

	styleComboBox(_combobox);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	_combobox->addItem("False");
	_combobox->addItem("True");

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);

	if (any != nullptr) {
		label = any->label();
		int index = any->data<bool>() ? 1 : 0;
		_combobox->setCurrentIndex(index);
		_observer = new BoolObserver(_combobox);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;

	if (useLabel) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_combobox);

	QObject::connect(_combobox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &DfxQBoolControl::updatePath);

	setLayout(_layout);
}

DfxQBoolControl::~DfxQBoolControl() {
	delete _observer;
}

void DfxQBoolControl::updatePath(const QString &text) {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		bool val = _combobox->currentIndex() == 0 ? false : true;
		any->setData<bool>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxQBoolControl::styleComboBox(QComboBox *_combobox) {
	_combobox->setStyleSheet(
		"QComboBox{ "
		"color: white;"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"border-color: rgb(50,50,50);"
		"border-width: 2px;"
		"border-style: solid;"
		"border-radius: 4px;"
		"padding-left: 5px;"
		"}");

	_combobox->setStyleSheet(_combobox->styleSheet() +
		"QComboBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_combobox->setStyleSheet(_combobox->styleSheet() +
		"QComboBox::down-arrow{"
		"image: url(:/icon/triangle_arrow_down);"
		"width: 7px;"
		"height: 7px;"
		"}");

	_combobox->setStyleSheet(_combobox->styleSheet() +
		"QComboBox::drop-down{"
		"border-width: 0px;"
		"}");

	_combobox->setStyleSheet(_combobox->styleSheet() +
		"QComboBox:item:selected{"
		"color: black;"
		"padding-left: 20px;"
		"font-weight: bold; "
		"background: rgb(255,127,42);"
		"}");


}

QComboBox *DfxQBoolControl::comboBox() {
	return _combobox;
}

void DfxQBoolControl::addItem(int index, QString name) {
	_combobox->insertItem(index, name);
}

int DfxQBoolControl::selectedIndex() {
	return _combobox->currentIndex();
}

DfxQBoolControl::BoolObserver::BoolObserver(QComboBox *cb) {
	combo = cb;
}

void DfxQBoolControl::BoolObserver::update(fl::Any *any) {
	if (any != nullptr) {
		int val = any->data<bool>() ? 1 : 0;
		combo->setCurrentIndex(val);
	}
}