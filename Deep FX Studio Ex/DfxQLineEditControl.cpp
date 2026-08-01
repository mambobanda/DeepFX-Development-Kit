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

#include "DfxQLineEditControl.h"


DfxQLineEditControl::~DfxQLineEditControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}


DfxQLineEditControl::DfxQLineEditControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent) :DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_lineEdit = new QLineEdit();

	_layout->setMargin(0);

	style::styleQLineEdit(_lineEdit);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);

	if (any != nullptr) {
		label = any->label();
		_lineEdit->setText(any->data<std::string>().c_str());
		_observer = new LineEditObserver(_lineEdit);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;


	if (useLabel) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_lineEdit);

	QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, &DfxQLineEditControl::updatePath);

	setLayout(_layout);
}

void DfxQLineEditControl::updatePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		std::string val = _lineEdit->text().toStdString();
		any->setData<std::string>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxQLineEditControl::setValue(std::string value) {
	_lineEdit->setText(value.c_str());
	updatePath();
}

DfxQLineEditControl::LineEditObserver::LineEditObserver(QLineEdit *sp) {
	lineEdit = sp;
}

void DfxQLineEditControl::LineEditObserver::update(fl::Any *any) {

	if (any != nullptr) {
		std::string val = any->data<std::string>();
		lineEdit->setText(any->data<std::string>().c_str());
	}
}

