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

#include "DfxConsoleBuffer.h"

std::mutex DfxConsoleBuffer::_guard;

DfxConsoleBuffer *DfxConsoleBuffer::instance = nullptr;

DfxConsoleBuffer::DfxConsoleBuffer() {
	_maxBufferSize = 50000;
}

DfxConsoleBuffer *DfxConsoleBuffer::getInstance() {
	if (instance == nullptr)
		instance = new DfxConsoleBuffer();

	return instance;
}

void DfxConsoleBuffer::appendBuffer(DfxString str) {
	appendBuffer(str.c_str());
}

void DfxConsoleBuffer::appendBuffer(std::string str) {
	appendBuffer(str.c_str());
}

void DfxConsoleBuffer::appendBuffer(const char* str) {
	const std::lock_guard<std::mutex> lock(_guard);

	buffer << str;
	cullBuffer();

	QString tmpBuffer(buffer.str().c_str());
	bufferUpdateEvent(tmpBuffer);
}

void DfxConsoleBuffer::setMaxBufferSize(size_t bsize) {
	_maxBufferSize = bsize;
}

size_t DfxConsoleBuffer::maxBufferSize() {
	return _maxBufferSize;
}

void DfxConsoleBuffer::cullBuffer() {
	std::string tempStr = buffer.str();
	if (tempStr.size() > _maxBufferSize) {
		tempStr.erase(tempStr.begin(), tempStr.begin() + (tempStr.size() - _maxBufferSize));
		buffer.str("");
		buffer.clear();

		buffer << tempStr;
	}
}

void DfxConsoleBuffer::saveBufferToFile(DfxString, DfxString) {

}

std::string DfxConsoleBuffer::toString() {
	return buffer.str();
}