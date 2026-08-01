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

#include "DfxAny.h"


DfxAny::DfxAny() {
	_data_ = nullptr;
}


void *DfxAny::getData() {
	return _data_;
}

void DfxAny::setData(void *dat) {
	_data_ = dat;
}

bool DfxAny::isNull() {
	return (_data_ == nullptr);
}

void DfxAny::toNull() {
	_data_ = nullptr;
}

