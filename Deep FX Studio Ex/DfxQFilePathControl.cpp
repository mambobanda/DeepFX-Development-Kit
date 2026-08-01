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

#include <QSettings>
#include "DfxQFilePathControl.h"

DfxQFilePathControl::DfxQFilePathControl(std::string vpath, bool open, bool directory, std::string title, std::string root_dir, std::string type, std::string flabel, bool useLabel, QWidget *parent) {

	_layout = new QHBoxLayout();
	_label = nullptr;
	_lineEdit = new QLineEdit();
	_sceneGraphDialog = new QPushButton(QPixmap(":/icon/search_icon").scaledToHeight(14), "");

	_open = open;
	_directory = directory;
	_title = title;
	_root_dir = root_dir;
	_type = type;

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

	QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, &DfxQFilePathControl::updatePath);
	QObject::connect(_sceneGraphDialog, &QPushButton::clicked, this, &DfxQFilePathControl::getAttributePath);

	setLayout(_layout);
}

DfxQFilePathControl::~DfxQFilePathControl() {
	delete _observer;
	setStyleSheet("background-color: transparent;");
}

void DfxQFilePathControl::styleControls() {
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

QLineEdit *DfxQFilePathControl::lineEdit() {
	return _lineEdit;
}

void DfxQFilePathControl::setValue(std::string value) {

}

void DfxQFilePathControl::setRootDir(std::string value) {
	_root_dir = value;
}

void DfxQFilePathControl::updatePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(valuePath());

	if (any != nullptr) {
		std::string val = _lineEdit->text().toStdString();
		any->setData<std::string>(val);
		any->notifyObservers(_observer);

		DfxGlobal::command::exec("updateScene");
	}
}

void DfxQFilePathControl::getAttributePath() {
	QFileDialog diag;

	QString file("");
	if (_open) {
		if (_directory) {
			file = QFileDialog::getExistingDirectory(this, QString(_title.c_str()), QString(_root_dir.c_str()));
		}
		else {
			file = QFileDialog::getOpenFileName(this, QString(_title.c_str()), QString(_root_dir.c_str()), QString(_type.c_str()));
		}
	}
	else {
		if (_directory) {
			file = QFileDialog::getExistingDirectory(this, QString(_title.c_str()), QString(_root_dir.c_str()));
		}
		else {
			file = QFileDialog::getSaveFileName(this, QString(_title.c_str()), QString(_root_dir.c_str()), QString(_type.c_str()));
		}
	}
	
	_lineEdit->setText(file);

	emit rootDirChanged(file.toStdString());
	emit _lineEdit->editingFinished();
}

DfxQFilePathControl::LineEditObserver::LineEditObserver(QLineEdit *sp) {
	lineEdit = sp;
}

void DfxQFilePathControl::LineEditObserver::update(fl::Any *any) {

	if (any != nullptr) {
		std::string val = any->data<std::string>();
		lineEdit->setText(any->data<std::string>().c_str());
	}
}