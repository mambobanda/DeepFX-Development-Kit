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

#pragma once

#include <vector>
#include <QImage>
#include "DfxEntity.h"


class DfxRenderInterface: public DfxEntity {
private:
	uint32_t width;
	uint32_t height;

public:
	DfxRenderInterface(DfxGlobalEntityName uniquename);
	virtual ~DfxRenderInterface();

	virtual void newFrame(std::vector<size_t> nodes) = 0;
	virtual void render()	= 0;
	virtual void cleanUp()	= 0;
	virtual int  iterations() = 0;
	virtual int  iterationsUpdateTrigger() = 0;
	virtual bool available() = 0;

	virtual unsigned char *getFinalImageBuffer() = 0;
	virtual unsigned int getFinalImageBufferSize() = 0;
	virtual QImage getFinalImage() = 0;
public:

	uint32_t getWidth();
	void setWidth(uint32_t);

	uint32_t getHeight();
	void setHeight(uint32_t);
};