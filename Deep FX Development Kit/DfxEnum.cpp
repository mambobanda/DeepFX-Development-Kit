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
#include "DfxEnum.h"



DfxEnum::DfxEnum() {
	currentIndex = 0;
}

void DfxEnum::add(std::string name) {
	enumerator.push_back(name);
}

void DfxEnum::setCurrentIndex(size_t inx) {
	currentIndex = inx;
}

size_t DfxEnum::getCurrentIndex() {
	return currentIndex;
}

std::string DfxEnum::get(size_t inx) {
	return enumerator[inx];
}


size_t DfxEnum::size() {
	return enumerator.size();
}