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


#include "DfxQComboBoxControl.h"

DfxQComboBoxControl::DfxQComboBoxControl(std::string flabel, QWidget *parent):DfxBaseControl(parent) {

	_observer = nullptr;
	_layout = new QHBoxLayout();
	_label = nullptr;
	_combobox = new QComboBox(this);

	_layout->setMargin(0);

	styleComboBox(_combobox);

	style::styleQScrollBar(_combobox->view()->horizontalScrollBar());
	style::styleQScrollBar(_combobox->view()->verticalScrollBar());

	std::string label = flabel;

	if (label.length() > 0) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_combobox);

	setLayout(_layout);
}

DfxQComboBoxControl::DfxQComboBoxControl(std::string vpath, std::string flabel, std::vector<std::pair<std::string, unsigned int>> enums, bool useLabel, QWidget *parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_combobox = new QComboBox(this);

	_enumList = enums;

	_layout->setMargin(0);

	styleComboBox(_combobox);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);
	
	int itemCounter = 0;
	for (auto p : enums) {
		_enumerators.insert(p);
		addItem(itemCounter++, QString(p.first.c_str()));
	}

	if (any != nullptr) {
		label = any->label();
		selectByEnum(any->data<unsigned int>());
		_observer = new IntegerObserver(_combobox, enums);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;


	if (useLabel) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_combobox);

	QObject::connect(_combobox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &DfxQComboBoxControl::updatePath);

	setLayout(_layout);
	
}

DfxQComboBoxControl::~DfxQComboBoxControl() {
	delete _observer;
}

void DfxQComboBoxControl::selectByEnum(unsigned int en) {
	selectByEnum(en, _combobox, _enumList);
}

void DfxQComboBoxControl::selectByEnum(unsigned int en, QComboBox *combo, std::vector<std::pair<std::string, unsigned int>> &enums) {
	int counter = 0;

	for (auto p : enums) {
		if (p.second == en) {
			combo->setCurrentIndex(counter);
			break;
		}
		counter++;
	}
}

unsigned int DfxQComboBoxControl::selectionEnum(QString str) {

	auto itr = _enumerators.find(str.toStdString());

	if (itr != _enumerators.end()) {
		return itr->second;
	}

	return 0;
}

void DfxQComboBoxControl::updatePath(const QString &text) {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		unsigned int val = selectionEnum(text);
		any->setData<unsigned int>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxQComboBoxControl::styleComboBox(QComboBox *_combobox) {

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

	_combobox->setStyleSheet(_combobox->styleSheet() +
		"QComboBox:on {"
		"padding-left: 20px;"
		"}");

}

QComboBox *DfxQComboBoxControl::comboBox() {
	return _combobox;
}

void DfxQComboBoxControl::addItem(int index, QString name) {
	_combobox->insertItem(index, name);
}

int DfxQComboBoxControl::selectedIndex() {
	return _combobox->currentIndex();
}

DfxQComboBoxControl::IntegerObserver::IntegerObserver(QComboBox *cb, std::vector<std::pair<std::string, unsigned int>> el) {
	combo = cb;
	enumList = el;
}

void DfxQComboBoxControl::IntegerObserver::update(fl::Any *any) {
	if (any != nullptr) {
		unsigned int val = any->data<unsigned int>();
		DfxQComboBoxControl::selectByEnum(val, combo, enumList);
	}
}