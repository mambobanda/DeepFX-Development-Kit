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
#pragma once

#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <qtguiglobal.h>
#include <DfxGlobal.h>
#include <DfxEntity.h>
#include <DfxEntityBasicPropertyTypes.h>
#include <limits>
#include "DfxQControlBase.h"


class DfxQCustomSpinnerControl : public QDoubleSpinBox, public DfxQControlBase {
	Q_OBJECT
protected:
	QString textFromValue(double value) const override;
	double  valueFromText(const QString &text) const override;
	unsigned int type;
public:
	enum { VALUE_A=0x100, VALUE_B, VALUE_C };
	DfxQCustomSpinnerControl(DfxEntity *ent, size_t propID, unsigned int type, float initialVal = .0f, QWidget *parent = Q_NULLPTR);

	public slots :
	void updateEntity();
};


class DfxQDecimal3SpinnerControl : public QWidget, public DfxQControlBase {
	Q_OBJECT
protected:
	
private:
	DfxQCustomSpinnerControl *xSpinner;
	DfxQCustomSpinnerControl *ySpinner;
	DfxQCustomSpinnerControl *zSpinner;

	QBoxLayout *layout;
public:
	DfxQDecimal3SpinnerControl(DfxEntity *ent, size_t propID, DfxEntityPropertyTypes::Float3 initialVal = DfxEntityPropertyTypes::Float3(), float width = 160.f, QWidget *parent = Q_NULLPTR);

public slots :
	void updateEntity();
};