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

#include "DfxQFluentPathControl.h"

DfxQFluentPathControl::DfxQFluentPathControl(std::string vpath, std::string flabel, bool useLabel, QWidget *parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_lineEdit = new QLineEdit();
	_sceneGraphDialog = new QPushButton(QPixmap(":/icon/search_icon").scaledToHeight(14), "");

	_layout->setMargin(0);
	_layout->setSpacing(1);

	styleControls();

	_sceneGraphDialog->setFixedWidth(20);

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
	_layout->addWidget(_sceneGraphDialog);

	QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, &DfxQFluentPathControl::updatePath);
	QObject::connect(_sceneGraphDialog, &QPushButton::clicked, this, &DfxQFluentPathControl::getAttributePath);

	setLayout(_layout);
}

DfxQFluentPathControl::~DfxQFluentPathControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}

void DfxQFluentPathControl::styleControls() {
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

	_sceneGraphDialog->setStyleSheet(rightButtonSty + hoverSty + hoverPressdSty);
}

void DfxQFluentPathControl::setValue(std::string value) {

}

void DfxQFluentPathControl::updatePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		std::string val = _lineEdit->text().toStdString();
		any->setData<std::string>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");

		emit fluentPathUpdated(val);
	}
}

void DfxQFluentPathControl::getAttributePath() {
	DfxSceneGraphSelectionDialog diag;

	auto rc = diag.exec();

	if (rc) {
		_lineEdit->setText(diag.referencePath().c_str());
		emit _lineEdit->editingFinished();
	}
}

DfxQFluentPathControl::LineEditObserver::LineEditObserver(QLineEdit *sp) {
	lineEdit = sp;
}

void DfxQFluentPathControl::LineEditObserver::update(fl::Any *any) {

	if (any != nullptr) {
		std::string val = any->data<std::string>();
		lineEdit->setText(any->data<std::string>().c_str());
	}
}