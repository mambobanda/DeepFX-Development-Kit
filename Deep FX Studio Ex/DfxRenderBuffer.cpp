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

#include "DfxRenderBuffer.h"

DfxRenderBuffer::DfxRenderBuffer() {
	_index = 0;
}

DfxRenderBuffer::~DfxRenderBuffer() {

}

void DfxRenderBuffer::setImage(QImage image) {
	const std::lock_guard<std::recursive_mutex> lock(_mutex);
	_buffer = image;
	emit updated();
}

QImage &DfxRenderBuffer::getImage() {
	const std::lock_guard<std::recursive_mutex> lock(_mutex);
	return _buffer;
}

std::string DfxRenderBuffer::bufferName() {
	return _bufferName;
}

void DfxRenderBuffer::setBufferName(std::string name) {
	_bufferName = name;
	emit updated();
}

void DfxRenderBuffer::setIndex(size_t inx) {
	_index = inx;
}

size_t DfxRenderBuffer::getIndex() {
	return _index;
}

void DfxRenderBuffer::lock() {
	_mutex.lock();
}

void DfxRenderBuffer::unlock() {
	_mutex.unlock();
}