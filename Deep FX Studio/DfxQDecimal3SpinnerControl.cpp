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

#include "DfxQDecimal3SpinnerControl.h"

DfxQCustomSpinnerControl::DfxQCustomSpinnerControl(DfxEntity *ent, size_t propID, unsigned int type, float initialVal, QWidget *parent ) : QDoubleSpinBox(parent){

	entity = ent;
	propertyID = propID;

	setDecimals(5);
	setMinimum(std::numeric_limits<float>::lowest());
	setMaximum(std::numeric_limits<float>::max());

	this->type = type;

	setValue(initialVal);

	QObject::connect(this, &DfxQCustomSpinnerControl::editingFinished, this, &DfxQCustomSpinnerControl::updateEntity);
}

void DfxQCustomSpinnerControl::updateEntity() {

	double val = value();

	if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

		if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

			if (type == VALUE_A) {
				((DfxEntityPropertyTypes::Float3*)entity->getProperty(propertyID))->x = (float)val;
			}
			else if (type == VALUE_B) {
				((DfxEntityPropertyTypes::Float3*)entity->getProperty(propertyID))->y = (float)val;
			}
			else if (type == VALUE_C) {
				((DfxEntityPropertyTypes::Float3*)entity->getProperty(propertyID))->z = (float)val;
			}
		
			entity->makeDirty();
			entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
			DfxGlobal::command::exec("updateViewport");
		}
	}
}

QString DfxQCustomSpinnerControl::textFromValue(double value) const {
	std::stringstream ss;
	ss << value;

	return QString(ss.str().c_str());
}

double DfxQCustomSpinnerControl::valueFromText(const QString &text) const {
	return text.toDouble();
}

DfxQDecimal3SpinnerControl::DfxQDecimal3SpinnerControl(DfxEntity *ent, size_t propID, DfxEntityPropertyTypes::Float3 initialVal, float width, QWidget *parent): QWidget(parent) {

	entity = ent;
	
	setFixedWidth(width);

	layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, this);
	layout->setMargin(0);

	float inval = ((DfxEntityPropertyTypes::Float3*)entity->getProperty(propID))->x;
	xSpinner = new DfxQCustomSpinnerControl(entity, propID, DfxQCustomSpinnerControl::VALUE_A, inval, this);
	
	inval = ((DfxEntityPropertyTypes::Float3*)entity->getProperty(propID))->y;
	ySpinner = new DfxQCustomSpinnerControl(entity, propID, DfxQCustomSpinnerControl::VALUE_B, inval, this);
	
	inval = ((DfxEntityPropertyTypes::Float3*)entity->getProperty(propID))->z;
	zSpinner = new DfxQCustomSpinnerControl(entity, propID, DfxQCustomSpinnerControl::VALUE_C, inval, this);

	layout->addWidget(xSpinner);
	layout->addWidget(ySpinner);
	layout->addWidget(zSpinner);

	setLayout(layout);
}

void DfxQDecimal3SpinnerControl::updateEntity() {

	//double val = value();

	//if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

	//	if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {
	//		/**((float*)entity->getProperty(propertyID)) = (float)val;
	//		entity->makeDirty();
	//		entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();*/
	//		DfxGlobal::command::exec("updateViewport");
	//	}
	//}
}