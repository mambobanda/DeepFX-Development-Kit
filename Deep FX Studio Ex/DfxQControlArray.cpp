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

#include "DfxQControlArray.h"


DfxQControlArray::~DfxQControlArray() {
	
}

DfxQControlArray::DfxQControlArray(QWidget *parent): QWidget(parent) {
	_formLayout = new QFormLayout();
	_formLayout->setLabelAlignment(Qt::AlignRight);
	_formLayout->setVerticalSpacing(2);
	setLayout(_formLayout);
}

void DfxQControlArray::addControl(std::string label, QWidget *control) {
	_formLayout->addRow(label.c_str(), control);
}

void DfxQControlArray::addControl(QWidget *control, std::string label) {
	_formLayout->addRow(control, new QLabel(label.c_str()));
}


void DfxQControlArray::addControl(QWidget *control) {
	_formLayout->addRow(control);
}

void DfxQControlArray::addSpacer(int width, int height) {
	_formLayout->addItem(new QSpacerItem(width, height, QSizePolicy::Preferred, QSizePolicy::Fixed));
}