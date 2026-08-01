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

#include "DfxQIntegerSpinnerControl.h"


DfxQIntegerSpinnerControl::~DfxQIntegerSpinnerControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}

void DfxQIntegerSpinnerControl::styleSpinner(QSpinBox *spinner) {

	spinner->setStyleSheet(
		"QSpinBox { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 5px;"
		"padding-left: 2px;"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QSpinBox:focus { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QSpinBox::down-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center left;"
		"image: url(:/icon/spinner_left);"
		"padding-left: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");

	spinner->setStyleSheet(spinner->styleSheet() +
		"QSpinBox::up-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center right;"
		"image: url(:/icon/spinner_right);"
		"padding-right: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");
}

DfxQIntegerSpinnerControl::DfxQIntegerSpinnerControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent, unsigned int type) :DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_spinner = new CustomSpinner();

	_type = type;

	_layout->setMargin(0);

	_spinner->setRange(-std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
	_spinner->setSingleStep(1);
	_spinner->setAlignment(Qt::AlignCenter);

	styleSpinner(_spinner);

	std::string label = "Control";
	setValuePath(vpath);
	_fixedLabel = flabel;

	auto any = DfxSpace::getInstance()->findAbsolute(vpath);

	if (any != nullptr) {
		label = any->label();

		if(_type == INT) _spinner->setValue(any->data<int>());
		else if(_type == UNSIGNED_INT) _spinner->setValue((int)any->data<unsigned int>());
		else if(_type == SIZE_T) _spinner->setValue((int)any->data<size_t>());

		_observer = new IntegerObserver(_spinner, _type);
		any->addObserver(_observer);
	}

	if (_fixedLabel.length() > 0) label = _fixedLabel;


	if (useLabel) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}

	_layout->addWidget(_spinner);

	QObject::connect(_spinner, &QDoubleSpinBox::editingFinished, this, &DfxQIntegerSpinnerControl::updatePath);

	setLayout(_layout);
}

void DfxQIntegerSpinnerControl::updatePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		int val = _spinner->value();

		if (_type == INT) any->setData<int>(val);
		else if (_type == UNSIGNED_INT) any->setData<unsigned int>((unsigned int)val);
		else if (_type == SIZE_T) any->setData<size_t>((size_t)val);

		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxQIntegerSpinnerControl::setValue(int value) {
	_spinner->setValue(value);
	updatePath();
}

int DfxQIntegerSpinnerControl::maxValue() {
	return _maxValue;
}

void DfxQIntegerSpinnerControl::setMaxValue(int mx) {
	_maxValue = mx;
	_spinner->setMaximum(mx);
}

int DfxQIntegerSpinnerControl::minValue() {
	return _minValue;
}

void DfxQIntegerSpinnerControl::setMinValue(int min) {
	_minValue = min;
	_spinner->setMinimum(min);
}

DfxQIntegerSpinnerControl::IntegerObserver::IntegerObserver(QSpinBox *sp, unsigned int type) {
	spinner = sp;
	_type = type;
}

void DfxQIntegerSpinnerControl::IntegerObserver::update(fl::Any *any) {
	if (any != nullptr) {
		int val = 0;
		
		if (_type == DfxQIntegerSpinnerControl::INT) val = any->data<int>();
		else if (_type == DfxQIntegerSpinnerControl::UNSIGNED_INT) val = (int)any->data<unsigned int>();
		else if (_type == DfxQIntegerSpinnerControl::SIZE_T) val = (int)any->data<size_t>();

		spinner->setValue(val);
	}
}

DfxQIntegerSpinnerControl::CustomSpinner::CustomSpinner(QWidget *parent) :QSpinBox(parent) {
	mousePressed = false;
	lastX = 0;
}


//QString DfxQIntegerSpinnerControl::CustomSpinner::textFromValue(double value) const {
//	std::stringstream ss;
//	ss << value;
//
//	return QString(ss.str().c_str());
//}
//
//double DfxQIntegerSpinnerControl::CustomSpinner::valueFromText(const QString &text) const {
//	return text.toDouble();
//}

void DfxQIntegerSpinnerControl::CustomSpinner::stepBy(int steps) {
	setValue(value() + (singleStep() * steps));
	emit editingFinished();
}

void DfxQIntegerSpinnerControl::CustomSpinner::mousePressEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(Qt::BlankCursor);
	mousePressed = true;

	lastX = event->pos().x();
}

void DfxQIntegerSpinnerControl::CustomSpinner::mouseReleaseEvent(QMouseEvent *event) {
	QApplication::setOverrideCursor(Qt::ArrowCursor);
	mousePressed = false;
}

void DfxQIntegerSpinnerControl::CustomSpinner::mouseMoveEvent(QMouseEvent *event) {
	if (mousePressed) {
		int offX = (event->pos().x() - lastX);

		setValue(value() + (singleStep() * offX));

		lastX = event->pos().x();

		emit editingFinished();
	}
}