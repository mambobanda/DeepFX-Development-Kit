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

#include "DfxQVectorControl.h"

DfxQVectorControl::DfxQVectorControl(std::string vpath) {

}

DfxQVectorControl::~DfxQVectorControl() {
	delete _observerX;
	delete _observerY;
	delete _observerZ;
}

DfxQVectorControl::DfxQVectorControl(std::string vpathR, std::string flabel, bool useLabel, QWidget *parent) :DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_spinnerLayout = new QHBoxLayout();
	_xSpinner = new CustomSpinner();
	_ySpinner = new CustomSpinner();
	_zSpinner = new CustomSpinner();
	_label = nullptr;

	_layout->setMargin(0);

	_xSpinner->setAlignment(Qt::AlignCenter);
	_xSpinner->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_xSpinner->setDecimals(6);
	_xSpinner->setSingleStep(.1);
	_xSpinner->setMinimumWidth(30);

	_ySpinner->setAlignment(Qt::AlignCenter);
	_ySpinner->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_ySpinner->setDecimals(6);
	_ySpinner->setSingleStep(.1);
	_ySpinner->setMinimumWidth(30);

	_zSpinner->setAlignment(Qt::AlignCenter);
	_zSpinner->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_zSpinner->setDecimals(6);
	_zSpinner->setSingleStep(.1);
	_zSpinner->setMinimumWidth(30);

	
	_xSpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);
	_ySpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);
	_zSpinner->setButtonSymbols(QAbstractSpinBox::NoButtons);
	

	std::string label = "Control";

	_path = vpathR;;

	_fixedLabel = flabel;

	if (useLabel) {
		_label = new QLabel((_fixedLabel + ":").c_str());
		_layout->addWidget(_label);
	}

	_spinnerLayout->setMargin(0);
	_spinnerLayout->setSpacing(2);

	_spinnerLayout->addWidget(_xSpinner);
	_spinnerLayout->addWidget(_ySpinner);
	_spinnerLayout->addWidget(_zSpinner);

	_layout->addLayout(_spinnerLayout);

	auto any00 = DfxSpace::getInstance()->findAbsolute(_path);
	
	if (any00 != nullptr) {

		glm::vec3 vec = any00->data<glm::vec3>();	

		_xSpinner->setValue(vec.x);
		_ySpinner->setValue(vec.y);
		_zSpinner->setValue(vec.z);

		_observerX = new FloatObserver(_xSpinner, _ySpinner, _zSpinner);
		_observerY = new FloatObserver(_xSpinner, _ySpinner, _zSpinner);
		_observerZ = new FloatObserver(_xSpinner, _ySpinner, _zSpinner);

		any00->addObserver(_observerX);
		any00->addObserver(_observerY);
		any00->addObserver(_observerZ);

	}

	styleSpinner(_xSpinner, X, LEFT_ROUNDED);
	styleSpinner(_ySpinner, Y, NO_ROUNDING);
	styleSpinner(_zSpinner, Z, RIGHT_ROUNDED);

	QObject::connect(_xSpinner, &QDoubleSpinBox::editingFinished, this, &DfxQVectorControl::updatePath);
	QObject::connect(_ySpinner, &QDoubleSpinBox::editingFinished, this, &DfxQVectorControl::updatePath);
	QObject::connect(_zSpinner, &QDoubleSpinBox::editingFinished, this, &DfxQVectorControl::updatePath);

	setLayout(_layout);
}

void DfxQVectorControl::styleSpinner(QDoubleSpinBox *spinner, uint primaryColor, uint rcorner) {

	std::stringstream ss_qsbox;
	ss_qsbox << "QDoubleSpinBox { ";
	ss_qsbox << "border: 0px solid;";
	ss_qsbox << "background-color: rgb(70,70,70);";
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
		"QDoubleSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");
}

void DfxQVectorControl::updatePath() {
	auto any00 = DfxSpace::getInstance()->findAbsolute(_path);

	if (any00 != nullptr) {

		float x	= _xSpinner->value();
		float y = _ySpinner->value();
		float z	= _zSpinner->value();

		any00->setData<glm::vec3>(glm::vec3(x,y,z));
	
		std::vector<fl::Observer*> obs;
		obs.push_back(_observerX);
		obs.push_back(_observerY);
		obs.push_back(_observerZ);

		any00->notifyObservers(obs);

		DfxGlobal::command::exec("updateScene");
	}
}

DfxQVectorControl::FloatObserver::FloatObserver(QDoubleSpinBox *a, QDoubleSpinBox *b, QDoubleSpinBox *c) {

	spinnerA = a;
	spinnerB = b;
	spinnerC = c;

}

void DfxQVectorControl::FloatObserver::update(fl::Any *any) {

	if (any != nullptr) {

		glm::vec3 val = any->data<glm::vec3>();

		spinnerA->setValue(val.x);
		spinnerB->setValue(val.y);
		spinnerC->setValue(val.z);

	}
}

