
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

#include "DfxRenderInterface.h"

DfxRenderInterface::DfxRenderInterface(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {

}

DfxRenderInterface::~DfxRenderInterface() {

}

uint32_t DfxRenderInterface::getWidth() {
	return width;
}

void DfxRenderInterface::setWidth(uint32_t w) {
	width = w;
}

uint32_t DfxRenderInterface::getHeight() {
	return height;
}

void DfxRenderInterface::setHeight(uint32_t h) {
	height = h;
}