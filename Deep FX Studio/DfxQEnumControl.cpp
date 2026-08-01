/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxQEnumControl.h"



DfxQEnumControl::DfxQEnumControl(DfxEntity *ent, size_t propID, float width,
	QWidget *parent) : QComboBox(parent) {

	entity = ent;
	propertyID = propID;

	setFixedWidth(width);

	DfxEnum *en = (DfxEnum*)entity->getProperty(propertyID);
	
	for (int i = 0; i < en->size(); i++) {
		insertItem(i, en->get(i).c_str());
	}

	setCurrentIndex(en->getCurrentIndex());

	QObject::connect(this, QOverload<int>::of(&DfxQEnumControl::currentIndexChanged),
		this, &DfxQEnumControl::updateEntity);
}


void DfxQEnumControl::updateEntity(int d) {

	if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

		if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {
			((DfxEnum*)entity->getProperty(propertyID))->setCurrentIndex(d);
			entity->makeDirty();
			entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
			DfxGlobal::command::exec("updateViewport");
		}
	}
}

