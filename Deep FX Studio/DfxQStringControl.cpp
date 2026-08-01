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

#include "DfxQStringControl.h"

DfxQStringControl::DfxQStringControl(DfxEntity *ent, size_t propID, std::string initialVal,
	float width, QWidget *parent) : QLineEdit(parent) {

	entity = ent;
	propertyID = propID;

	setText(((std::string*)ent->getProperty(propID))->c_str());
	setFixedWidth(width);

	QObject::connect(this, &QLineEdit::editingFinished, this, &DfxQStringControl::updateEntity);
}


void DfxQStringControl::updateEntity() {

	std::string val = text().toStdString();

	if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

		if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE) {
			*((std::string*)entity->getProperty(propertyID)) = val;

			entity->makeDirty();
			entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
			DfxGlobal::command::exec("updateViewport");
		}
	}
}
