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
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"


class DfxQColorControl : public DfxBaseControl {
private:

	class IntegerObserver : public fl::Observer {
	public:
		QSpinBox *spinner;

		IntegerObserver(QSpinBox *);
		void update(fl::Any *any) override;
	};

private:

	enum MyEnum { RED=0, GREEN, BLUE, LEFT_ROUNDED, RIGHT_ROUNDED, NO_ROUNDING};


	IntegerObserver *_observerR;
	IntegerObserver *_observerG;
	IntegerObserver *_observerB;

	QSpinBox *_rSpinner;
	QSpinBox *_gSpinner;
	QSpinBox *_bSpinner;

	QPushButton *_colorButton;

	QHBoxLayout *_layout;
	QHBoxLayout *_spinnerLayout;
	QLabel *_label;

	QColor _currentColor;

	std::string _vpathR;
	std::string _vpathG;
	std::string _vpathB;

	std::string _fixedLabel;

	void styleColorButton(QPushButton *btn);
	void styleSpinner(QSpinBox *spinner, uint primaryColor, uint rounded_corner);
public:
	DfxQColorControl(std::string vpathR, std::string vpathG, std::string vpathB );
	DfxQColorControl(std::string vpathR, std::string vpathG, std::string vpathB, std::string flabel = "", bool useLabel = true, QWidget *parent = nullptr);
	~DfxQColorControl();
	
	static int floatToIntColor(float color);
	static float intToFloatColor(int color);

public slots:
	void colorPickerAction();
	void updatePath();
};