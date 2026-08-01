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

#include "DfxQDecimalSpinnerControl.h"

DfxQDecimalSpinnerControl::DfxQDecimalSpinnerControl(DfxEntity *ent, size_t propID, float initialVal, float width,
	QWidget *parent): QDoubleSpinBox(parent) {

	entity = ent;
	propertyID = propID;

	setDecimals(5);
	setMinimum(std::numeric_limits<float>::lowest());
	setMaximum(std::numeric_limits<float>::max());

	setValue(initialVal);
	setFixedWidth(width);

	QObject::connect(this, &DfxQDecimalSpinnerControl::editingFinished,
					 this, &DfxQDecimalSpinnerControl::updateEntity);
}


void DfxQDecimalSpinnerControl::updateEntity() {
	
	double val = value();

	if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

		if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {
			*((float*)entity->getProperty(propertyID)) = (float)val;
			entity->makeDirty();
			entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
			DfxGlobal::command::exec("updateViewport");
		}
	}
}

QString DfxQDecimalSpinnerControl::textFromValue(double value) const {
	std::stringstream ss;
	ss << value;

	return QString(ss.str().c_str());
}

double DfxQDecimalSpinnerControl::valueFromText(const QString &text) const {
	return text.toDouble();
}