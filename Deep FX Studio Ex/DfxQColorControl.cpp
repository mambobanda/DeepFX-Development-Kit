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

#include "DfxQColorControl.h"

DfxQColorControl::DfxQColorControl(std::string vpathR, std::string vpathG, std::string vpathB) {
	
}

DfxQColorControl::~DfxQColorControl() {
	delete _observerR;
	delete _observerG;
	delete _observerB;
}

DfxQColorControl::DfxQColorControl(std::string vpathR, std::string vpathG, std::string vpathB, std::string flabel, bool useLabel, QWidget *parent):DfxBaseControl(parent) {
	
	_layout = new QHBoxLayout();
	_spinnerLayout = new QHBoxLayout();
	_rSpinner = new QSpinBox();
	_gSpinner = new QSpinBox();
	_bSpinner = new QSpinBox();
	_colorButton = new QPushButton();
	_label = nullptr;

	_layout->setMargin(0);

	_rSpinner->setRange(0, 255);
	_gSpinner->setRange(0, 255);
	_bSpinner->setRange(0, 255);

	_rSpinner->setAlignment(Qt::AlignCenter);
	_gSpinner->setAlignment(Qt::AlignCenter);
	_bSpinner->setAlignment(Qt::AlignCenter);

	_rSpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);
	_gSpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);
	_bSpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);

	std::string label = "Control";

	_vpathR = vpathR;
	_vpathG = vpathG;
	_vpathB = vpathB;

	_fixedLabel = flabel;

	if (useLabel) {
		_label = new QLabel((_fixedLabel + ":").c_str());
		_layout->addWidget(_label);
	}

	_spinnerLayout->setMargin(0);
	_spinnerLayout->setSpacing(0);

	_spinnerLayout->addWidget(_rSpinner);
	_spinnerLayout->addWidget(_gSpinner);
	_spinnerLayout->addWidget(_bSpinner);
	_spinnerLayout->addWidget(_colorButton);

	_layout->addLayout(_spinnerLayout);

	auto any00 = DfxSpace::getInstance()->findAbsolute(_vpathR);
	auto any01 = DfxSpace::getInstance()->findAbsolute(_vpathG);
	auto any02 = DfxSpace::getInstance()->findAbsolute(_vpathB);

	if (any00 != nullptr && any01 != nullptr && any02 != nullptr) {

		int red   = floatToIntColor(any00->data<float>());
		int blue  = floatToIntColor(any01->data<float>());
		int green = floatToIntColor(any02->data<float>());

		_rSpinner->setValue(red);
		_gSpinner->setValue(blue);
		_bSpinner->setValue(green);

		_observerR = new IntegerObserver(_rSpinner);
		_observerG = new IntegerObserver(_gSpinner);
		_observerB = new IntegerObserver(_bSpinner);

		any00->addObserver(_observerR);
		any01->addObserver(_observerG);
		any02->addObserver(_observerB);

		_currentColor = QColor(red, blue, green);
	}

	styleColorButton(_colorButton);

	styleSpinner(_rSpinner, RED, LEFT_ROUNDED);
	styleSpinner(_gSpinner, GREEN, NO_ROUNDING);
	styleSpinner(_bSpinner, BLUE, NO_ROUNDING);

	QObject::connect(_rSpinner, &QSpinBox::editingFinished, this, &DfxQColorControl::updatePath);
	QObject::connect(_gSpinner, &QSpinBox::editingFinished, this, &DfxQColorControl::updatePath);
	QObject::connect(_bSpinner, &QSpinBox::editingFinished, this, &DfxQColorControl::updatePath);

	QObject::connect(_colorButton, &QPushButton::clicked, this, &DfxQColorControl::colorPickerAction);

	setLayout(_layout);
}

void DfxQColorControl::styleColorButton(QPushButton *btn) {
	std::stringstream ss_qbtn;
	ss_qbtn << "QPushButton{ ";
	ss_qbtn << "border: 2px solid;";
	ss_qbtn << "border-color: rgb(" << _currentColor.red() << "," << _currentColor.green() << "," << _currentColor.blue() << ");";
	ss_qbtn << "background-color: rgb(" << _currentColor.red() << "," << _currentColor.green() << "," << _currentColor.blue() << ");";
	ss_qbtn << "border-top-right-radius: 4px;";
	ss_qbtn << "border-bottom-right-radius: 4px;";
	ss_qbtn << "}";

	btn->setStyleSheet(ss_qbtn.str().c_str());
}

void DfxQColorControl::styleSpinner(QSpinBox *spinner, uint primaryColor, uint rcorner) {
	
	std::stringstream ss_qsbox;
	ss_qsbox << "QSpinBox { ";
	ss_qsbox << "border: 0px solid;";
	ss_qsbox << "background-color: rgb(70,70,70);";
	if(primaryColor == RED) ss_qsbox << "color: rgb(255,90,90);";
	else if(primaryColor == GREEN) ss_qsbox << "color: rgb(90,255,90);";
	else if(primaryColor == BLUE) ss_qsbox << "color: rgb(90,90,255);";
	if (rcorner == LEFT_ROUNDED) {
		ss_qsbox << "border-top-left-radius: 4px;";
		ss_qsbox << "border-bottom-left-radius: 4px;";
	}
	else if (rcorner == RIGHT_ROUNDED) {
		ss_qsbox << "border-top-right-radius: 4px;";
		ss_qsbox << "border-bottom-right-radius: 4px;";
	}
	ss_qsbox << "}";

	spinner->setStyleSheet(ss_qsbox.str().c_str());

	spinner->setStyleSheet(spinner->styleSheet() +
		"QSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");
}

float DfxQColorControl::intToFloatColor(int color) {
	return (float)color / 255.f;
}

int DfxQColorControl::floatToIntColor(float color) {
	return color * 255.f;
}

void DfxQColorControl::colorPickerAction() {
	auto _newColor = QColorDialog::getColor(_currentColor, this);
	
	if (_newColor.isValid()) _currentColor = _newColor;

	styleColorButton(_colorButton);
	
	_rSpinner->setValue(_currentColor.red());
	_gSpinner->setValue(_currentColor.green());
	_bSpinner->setValue(_currentColor.blue());

	updatePath();
}

void DfxQColorControl::updatePath() {
	auto any00 = DfxSpace::getInstance()->findAbsolute(_vpathR);
	auto any01 = DfxSpace::getInstance()->findAbsolute(_vpathG);
	auto any02 = DfxSpace::getInstance()->findAbsolute(_vpathB);

	if (any00 != nullptr && any00 != nullptr && any00 != nullptr) {

		float red = intToFloatColor(_rSpinner->value());
		float green = intToFloatColor(_gSpinner->value());
		float blue = intToFloatColor(_bSpinner->value());
		
		_currentColor = QColor(_rSpinner->value(), _gSpinner->value(), _bSpinner->value());

		any00->setData<float>(red);
		any01->setData<float>(green);
		any02->setData<float>(blue);

		any00->notifyObservers(_observerR);
		any01->notifyObservers(_observerG);
		any02->notifyObservers(_observerB);


		styleColorButton(_colorButton);

		DfxGlobal::command::exec("updateScene");
	}
}

DfxQColorControl::IntegerObserver::IntegerObserver(QSpinBox *sp) {
	spinner = sp;
}

void DfxQColorControl::IntegerObserver::update(fl::Any *any) {
	if (any != nullptr) {
		float val = any->data<int>();
		spinner->setValue(any->data<int>());
	}
}
