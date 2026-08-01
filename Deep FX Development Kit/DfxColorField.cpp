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

#include "DfxColorField.h"

DfxColorField::DfxColorField(unsigned int rc, unsigned int gc, unsigned int bc) {
	setRGB(rc, gc, bc);
}

DfxColorField::DfxColorField(unsigned int value) {
	setValue(value);
}

DfxColorField::~DfxColorField() {

}

unsigned int DfxColorField::value() {
	return _c;
}

void DfxColorField::setValue(unsigned int value) {
	_c = value;

	assignRGB();
}

unsigned int DfxColorField::r() {
	return _r;
}

unsigned int DfxColorField::g() {
	return _g;
}

unsigned int DfxColorField::b() {
	return _b;
}

void DfxColorField::setRGB(unsigned int rc, unsigned int gc, unsigned int bc) {
	_r = rc;
	_g = gc;
	_b = bc;

	assignValue();
}

void DfxColorField::increment() {
	_c = _c + 1;
	assignRGB();
}

void DfxColorField::decrement() {
	_c = _c - 1;
	assignRGB();
}