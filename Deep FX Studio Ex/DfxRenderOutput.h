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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QObject>
#include <vector>
#include "DfxSpace.h"
#include "DfxRenderCommon.h"
#include "DfxRenderBuffer.h"

class DfxRenderOutput: public QObject {
	Q_OBJECT
private:
	DfxRenderNodeBase *_renderer;
	std::vector<DfxRenderBuffer*> _buffers;

	int _progress;
public:

	DfxRenderOutput(DfxRenderNodeBase *renderer);
	virtual ~DfxRenderOutput();

	void addBuffer(DfxRenderBuffer *);
	void removeBuffer(int inx);
	int bufferCount();
	DfxRenderBuffer *getBuffer(int inx);

	std::string rendererName();

	void notifyBuffersUpdated();

	void clear();

	int progress();
	void setProgress(int );

signals:
	void aboutToUpdateBuffer();
	void buffersUpdated();
	void buffersCleared();
	void progressUpdated(int prg);

public slots:
	void stopRenderer();
};