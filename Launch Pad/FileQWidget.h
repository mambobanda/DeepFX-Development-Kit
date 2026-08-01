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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <limits>
#include <QApplication>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <QFileDialog>
#include "Resource.h"


class FileQWidget : public QWidget {
private:
	std::string _fixedLabel;

	QHBoxLayout *_layout;
	QLabel *_label;
	QLineEdit *_lineEdit;
	QPushButton *_sceneGraphDialog;

	bool _open;
	bool _directory;
	std::string _title;
	std::string _root_dir;
	std::string _type;

	std::string *_resource;
public:

	FileQWidget(std::string *output, bool open, bool directory, std::string root_dir, std::string type, QWidget *parent = nullptr);
	~FileQWidget();

	void setValue(std::string value);

public slots:

	void updatePath();
	void getAttributePath();
};