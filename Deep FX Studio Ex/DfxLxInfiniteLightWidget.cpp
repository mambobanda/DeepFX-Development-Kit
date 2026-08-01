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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxLxInfiniteLightWidget.h"

std::string DfxLxInfiniteLightWidget::_uniqueName = "UID_LUX_INFINITE_LIGHT_WIDGET";

DfxLxInfiniteLightWidget::DfxLxInfiniteLightWidget() {

	deepsg::SgShape::setScale(0.1f);

	initShapes();
	setShapeType(DfxItemWidgetType::INFINITE_LIGHT);
	setShape(this);
}

DfxLxInfiniteLightWidget::~DfxLxInfiniteLightWidget() {
}

std::string DfxLxInfiniteLightWidget::uniqueName() {
	return _uniqueName;
}

std::string DfxLxInfiniteLightWidget::getUniqueName() {
	return _uniqueName;
}

void DfxLxInfiniteLightWidget::initShapes() {


}

std::vector<std::pair<QWidget*, QString>> DfxLxInfiniteLightWidget::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxLxInfiniteLightWidget::properties() {
	return new QWidget;
}