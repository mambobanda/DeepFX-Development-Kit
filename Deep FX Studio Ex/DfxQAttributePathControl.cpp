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

#include "DfxQAttributePathControl.h"

DfxQAttributePathControl::~DfxQAttributePathControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}

DfxQAttributePathControl::DfxQAttributePathControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent) :DfxBaseControl(parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_lineEdit = new QLineEdit();
	_attributeDialog = new QPushButton(QPixmap(":/icon/search_icon").scaledToHeight(14), "");

	_layout->setMargin(0);
	_layout->setSpacing(1);

	styleControls();

	_attributeDialog->setFixedWidth(20);

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
	_layout->addWidget(_attributeDialog);

	QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, &DfxQAttributePathControl::updatePath);
	QObject::connect(_attributeDialog, &QPushButton::clicked, this, &DfxQAttributePathControl::getAttributePath);

	setLayout(_layout);
}

void DfxQAttributePathControl::updatePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		std::string val = _lineEdit->text().toStdString();
		any->setData<std::string>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void  DfxQAttributePathControl::styleControls() {

	QString leftButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-left-radius: 4px;"
		"border-top-left-radius: 4px;"
		"padding-left: 2px;"
		"height: 20px;"
		"}";

	QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"height: 20px;"
		"}";

	QString rightButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-right-radius: 4px;"
		"border-top-right-radius: 4px;"
		"padding-left: 2px;"
		"height: 20px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";


	_attributeDialog->setStyleSheet(rightButtonSty + hoverSty + hoverPressdSty);
}

void DfxQAttributePathControl::setValue(std::string value) {
	_lineEdit->setText(value.c_str());
	updatePath();
}

DfxQAttributePathControl::LineEditObserver::LineEditObserver(QLineEdit *sp) {
	lineEdit = sp;
}

void DfxQAttributePathControl::LineEditObserver::update(fl::Any *any) {

	if (any != nullptr) {
		std::string val = any->data<std::string>();
		lineEdit->setText(any->data<std::string>().c_str());
	}
}

void DfxQAttributePathControl::getAttributePath() {

	DfxQAttributeDialog diag;

	auto rc = diag.exec();

	if (rc) {
		_lineEdit->setText(diag.attributePath().c_str());
		emit _lineEdit->editingFinished();
	}
}