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
#include <QMouseEvent>
#include <DfxGlobal.h>
#include "DfxQStyleLineEdit.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQLineEditControl : public DfxBaseControl {
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

public:

	DfxQLineEditControl(std::string vpath, std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr);
	~DfxQLineEditControl();

	void setValue(std::string value);
public slots:
	void updatePath();
};