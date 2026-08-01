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

#include "DfxPointItem.h"
#include <QThread>

DfxPointItem::DfxPointItem(DfxGlobalEntityName uniquename): DfxViewItem(DFX_POINT_ITEM, uniquename) {

	setLogicalName("Point View");

	*((float*)getProperty(inSize)) = 4.f;

	setPropertyState(inSize, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);

	glInitialized = false;

	getDataInstance()->makeClean();
}

DfxPointItem::~DfxPointItem() {

}

bool DfxPointItem::preprocessViewItem() {
	return true;
}

void DfxPointItem::preprocess() {
	// pingParents();
}

void DfxPointItem::draw() {
	initializeOpenGLFunctions();

	if (propertyHasBinding(inPoints)) {
		
		pingParents();
	
		using namespace DfxEntityPropertyTypes;

		float psize = *(float*)getProperty(inSize);
		Float3Array *points = (Float3Array*)getProperty(inPoints);

		if (!points->isNull()) {

			glDisable(GL_LIGHTING);
			glPointSize(psize);
			glColor3f(0.f, 1.0f, 0.f);
			for (int i = 0; i < points->size(); i++) {

				glBegin(GL_POINTS);
				glVertex3f((*points)[i].x, (*points)[i].y, (*points)[i].z);
				glEnd();
			}
			glEnable(GL_LIGHTING);
		}
	}
}

DfxBounds DfxPointItem::bounds() {
	return DfxBounds();
}

bool DfxPointItem::selectable() {
	return false;
}

void DfxPointItem::calculateBounds() {
	// Do nothing
}

bool DfxPointItem::isExportable() {
	return true;
}

void *DfxPointItem::creator(DfxGlobalEntityName uniquename) {
	return new DfxPointItem(uniquename);
}

void DfxPointItem::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxPointItem::exportData() {
	return mapEntityProperties(this);
}

void DfxPointItem::initialize() {

	DfxViewItem::initialize();

	DfxEntityDB::createProperty(inSize, "point_size", new DfxEntityPropertyTypes::FloatPropertyType());
	DfxEntityDB::createProperty(inPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
}