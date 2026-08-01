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

#include "DfxRenderOutput.h"
#include "DfxRenderNodeBase.h"

DfxRenderOutput::DfxRenderOutput(DfxRenderNodeBase *renderer) {
	_renderer = renderer;
	_progress = 0;
}

DfxRenderOutput::~DfxRenderOutput() {

}

void DfxRenderOutput::addBuffer(DfxRenderBuffer *buff) {
	emit aboutToUpdateBuffer();
	_buffers.push_back(buff);
}

void DfxRenderOutput::removeBuffer(int inx) {
	emit aboutToUpdateBuffer();
	_buffers.erase(_buffers.begin() + inx);
}

int DfxRenderOutput::bufferCount() {
	return _buffers.size();
}

DfxRenderBuffer *DfxRenderOutput::getBuffer(int inx) {
	return _buffers[inx];
}

void DfxRenderOutput::notifyBuffersUpdated() {
	emit buffersUpdated();
}

void DfxRenderOutput::clear() {
	std::vector<DfxRenderBuffer*> buffs;
	for (auto buff : _buffers) buffs.push_back(buff);
	_buffers.clear();
	for (auto buff : buffs) delete buff;

	emit buffersCleared();
}

void DfxRenderOutput::stopRenderer() {
	_renderer->stopRendering();
}

std::string DfxRenderOutput::rendererName() {
	auto any = DfxSpace::getInstance()->findAbsolute(_renderer->referencePath());

	if (any != nullptr) return any->label();

	return "Unknown Renderer";
}

int DfxRenderOutput::progress() {
	return _progress;
}

void DfxRenderOutput::setProgress(int prg) {
	_progress = prg;
	emit progressUpdated(prg);
}