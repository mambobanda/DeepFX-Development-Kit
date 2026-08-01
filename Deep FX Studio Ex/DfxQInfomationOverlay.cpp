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

#include "DfxQInfomationOverlay.h"

DfxQInfomationOverlay::DfxQInfomationOverlay(int position, QWidget *parent): QWidget(parent) {
	_layout = new QVBoxLayout;
	_layout->setMargin(0);

	setLayout(_layout);
}

DfxQInfomationOverlay::~DfxQInfomationOverlay() {

}

QBoxLayout *DfxQInfomationOverlay::layout() {
	return _layout;
}
