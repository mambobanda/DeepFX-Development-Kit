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
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QSpinBox>
#include <QMouseEvent>
#include <QPushButton>
#include <DfxGlobal.h>
#include <sstream>
#include <QColorDialog>
#include <glm/vec3.hpp>
#include "DfxQDecimalSpinnerControl.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"

class DfxQVectorControl : public DfxBaseControl {
private:

	class FloatObserver : public fl::Observer {
	public:
		QDoubleSpinBox *spinnerA;
		QDoubleSpinBox *spinnerB;
		QDoubleSpinBox *spinnerC;

		FloatObserver(QDoubleSpinBox *, QDoubleSpinBox *, QDoubleSpinBox *);
		void update(fl::Any *any) override;
	};

private:

	enum MyEnum { X = 0, Y, Z, LEFT_ROUNDED, RIGHT_ROUNDED, NO_ROUNDING };

	FloatObserver *_observerX;
	FloatObserver *_observerY;
	FloatObserver *_observerZ;
	
	CustomSpinner *_xSpinner;
	CustomSpinner *_ySpinner;
	CustomSpinner *_zSpinner;

	QBoxLayout *_layout;
	QBoxLayout *_spinnerLayout;
	QLabel *_label;

	QColor _currentColor;

	std::string _path;

	std::string _fixedLabel;

	void styleSpinner(QDoubleSpinBox *spinner, uint primaryColor, uint rounded_corner);
public:
	DfxQVectorControl(std::string vpath);
	DfxQVectorControl(std::string vpath, std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr);
	~DfxQVectorControl();


public slots:
	void updatePath();
};