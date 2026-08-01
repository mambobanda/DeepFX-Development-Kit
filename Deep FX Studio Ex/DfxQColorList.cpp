/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "DfxQColorList.h"

DfxQColorList::DfxQColorList() {

	index = 0;

	_colors.push_back(QColor(255, 0, 0));
	_colors.push_back(QColor(0, 255, 0));
	_colors.push_back(QColor(0, 0, 255));
	_colors.push_back(QColor(255, 255, 0));
	_colors.push_back(QColor(255, 0, 255));
	_colors.push_back(QColor(0, 255, 255));
	_colors.push_back(QColor(128, 0, 0));
	_colors.push_back(QColor(0, 128, 0));
	_colors.push_back(QColor(0, 0, 128));
	_colors.push_back(QColor(128, 0, 128));
	_colors.push_back(QColor(128, 128, 0));
	_colors.push_back(QColor(170, 68, 0));
	_colors.push_back(QColor(255, 153, 85));
	_colors.push_back(QColor(145, 111, 111));
	_colors.push_back(QColor(212, 170, 0));
	_colors.push_back(QColor(136, 170, 0));
	_colors.push_back(QColor(0, 212, 170));
	_colors.push_back(QColor(55, 200, 171));
	_colors.push_back(QColor(0, 170, 212));
	_colors.push_back(QColor(85, 0, 212));
	_colors.push_back(QColor(136, 0, 170));
	_colors.push_back(QColor(204, 170, 255));

}

QColor DfxQColorList::next() {

	if (index == _colors.size()) {
		index = 0;
	}

	return _colors[index++];

}

void DfxQColorList::reset() {
	index = 0;
}
