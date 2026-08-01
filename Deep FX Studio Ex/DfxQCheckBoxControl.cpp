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

#include "DfxQCheckBoxControl.h"



DfxQCheckBoxControl::DfxQCheckBoxControl(std::string vpathR) {

}
	
DfxQCheckBoxControl::DfxQCheckBoxControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent): DfxBaseControl(parent) {
	
	_layout = new QHBoxLayout();
	_checkbox = new QCheckBox();

	_layout->setMargin(0);

	styleCheckbox(_checkbox);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);

	if (any != nullptr) {
		label = any->label();
		_checkbox->setChecked(any->data<bool>());
		_observer = new BoolObserver(_checkbox);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;

	if (useLabel) {
		_checkbox->setText(label.c_str());
	}

	_layout->addWidget(_checkbox);

	QObject::connect(_checkbox, &QCheckBox::stateChanged, this, &DfxQCheckBoxControl::updatePath);

	setLayout(_layout);
}

DfxQCheckBoxControl::~DfxQCheckBoxControl() {
	delete _observer;
}

void DfxQCheckBoxControl::styleCheckbox(QCheckBox *checkbox) {
	
	checkbox->setStyleSheet(checkbox->styleSheet() +
		"QCheckBox::indicator{"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"border-color: rgb(50,50,50);"
		"border-width: 2px;"
		"border-style: solid;"
		"border-radius: 5px;"
		"width: 10px;"
		"height: 10px;"
		"}");

	checkbox->setStyleSheet(checkbox->styleSheet() +
		"QCheckBox::indicator:checked{"
		"image: url(:/icon/check);"
		"}");
}

void DfxQCheckBoxControl::updatePath(int state) {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		bool val = _checkbox->isChecked();
		any->setData<bool>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

DfxQCheckBoxControl::BoolObserver::BoolObserver(QCheckBox *cb) {
	checkbox = cb;
}

void DfxQCheckBoxControl::BoolObserver::update(fl::Any *any) {

	if (any != nullptr) {
	    bool val = any->data<bool>();
		checkbox->setChecked(val);
	}
}
