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

#include <numeric>
#include <algorithm>
#include <QCheckbox>
#include "DfxQCustomSpinner.h"
#include "DfxBaseControl.h"
#include "DfxSpace.h"
#include "DfxAny.h"
#include "FlObserver.h"
#include "DfxHistoryManager.h"
#include "DfxAnyHistoryAction.h"
#include "DfxQKeyCheckBox.h"


class DfxQDecimalSpinnerControl : public DfxBaseControl {
private:

	class FloatObserver : public fl::Observer {
	public:
		QDoubleSpinBox *spinner;
	
		FloatObserver(QDoubleSpinBox *);
		void update(fl::Any *any) override;
	};

private:
	
	std::string _fixedLabel;

	FloatObserver *_observer;
	QHBoxLayout *_layout;
	QLabel *_label;
	CustomSpinner *_spinner;
	DfxQKeyCheckBox *_keyToggle;

	void styleSpinner(QDoubleSpinBox *spinner, bool keyFramed = false);

	float _minValue;
	float _maxValue;
public:

	DfxQDecimalSpinnerControl(std::string title);
	DfxQDecimalSpinnerControl(std::string vpath, std::string flabel = "", bool useLabel = true, bool keyable = true, QWidget *parent = nullptr);
	~DfxQDecimalSpinnerControl();

	float maxValue();
	void setMaxValue(float);

	float minValue();
	void setMinValue(float);
	
	float minMax(float val);

	void setValue(float value);

	void updateState();
public slots:
	void updatePath();
	void pathUpdated(std::string pth);
	void updateKey(int state);
};