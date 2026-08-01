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

#include "FileQWidget.h"

FileQWidget::FileQWidget(std::string *output, bool open, bool directory, std::string root_dir, std::string type, QWidget *parent) {

	_resource = output;

	_layout = new QHBoxLayout();
	_label = nullptr;
	_lineEdit = new QLineEdit();
	_sceneGraphDialog = new QPushButton(QPixmap(":/LaunchPad/search_icon").scaledToHeight(14), "");

	_open = open;
	_directory = directory;
	_title = "File";
	_root_dir = root_dir;
	_type = type;

	_layout->setMargin(0);
	_layout->setSpacing(1);

	_sceneGraphDialog->setFixedWidth(20);

	std::string label = "Control";

	_lineEdit->setText(output->c_str());

	if (_fixedLabel.length() > 0) label = _fixedLabel;

	/*if (useLabel) {
		_label = new QLabel((label + ":").c_str());
		_layout->addWidget(_label);
	}*/

	_layout->addWidget(_lineEdit);
	_layout->addWidget(_sceneGraphDialog);

	QObject::connect(_lineEdit, &QLineEdit::editingFinished, this, &FileQWidget::updatePath);
	QObject::connect(_sceneGraphDialog, &QPushButton::clicked, this, &FileQWidget::getAttributePath);

	setLayout(_layout);
}

FileQWidget::~FileQWidget() {
}


void FileQWidget::setValue(std::string value) {
	_lineEdit->setText(value.c_str());
}

void FileQWidget::updatePath() {
	*_resource = _lineEdit->text().toStdString();
}

void FileQWidget::getAttributePath() {
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
	emit _lineEdit->editingFinished();
}