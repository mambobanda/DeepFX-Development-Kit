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
#include <qtguiglobal.h>
#include <DfxGlobal.h>
#include <DfxEntityBasicPropertyTypes.h>
#include "DfxQControlBase.h"
#include "DfxEntity.h"


class DfxQDecimalSpinnerControl: public QDoubleSpinBox, public DfxQControlBase{
	Q_OBJECT
protected:
	QString textFromValue(double value) const override;
	double  valueFromText(const QString &text) const override;
public:
	DfxQDecimalSpinnerControl (DfxEntity *ent, size_t propID, float initialVal = .0f, float width = 160.f, 
		QWidget *parent = Q_NULLPTR);

public slots :
	void updateEntity();
};