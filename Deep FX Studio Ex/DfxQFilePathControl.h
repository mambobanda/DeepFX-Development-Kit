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
#include <DfxGlobal.h>
#include "DfxQStyleLineEdit.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQFilePathControl : public DfxBaseControl {
	Q_OBJECT
private:

	class LineEditObserver : public fl::Observer {
	public:
		QLineEdit *lineEdit;

		LineEditObserver(QLineEdit *);
		void update(fl::Any *any) override;
	};


private:
	std::string _fixedLabel;

	LineEditObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	QLineEdit *_lineEdit;
	QPushButton *_sceneGraphDialog;

	bool _open;
	bool _directory;
	std::string _title;
	std::string _root_dir;
	std::string _type;

	void styleControls();

public:

	DfxQFilePathControl(std::string vpath, bool open, bool directory, std::string title, std::string root_dir, std::string type,  std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr);
	~DfxQFilePathControl();

	void setValue(std::string value);
	void setRootDir(std::string value);

	QLineEdit *lineEdit();

signals:
	void rootDirChanged(std::string );

public slots:

	void updatePath();
	void getAttributePath();
};