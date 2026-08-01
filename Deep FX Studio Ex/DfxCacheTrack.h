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

#include <string>
#include <VH5File.h>
#include <VH5DocumentResource.h>
#include <DfxGlobal.h>
#include "DfxCacheRecorder.h"
#include "DfxCachePlayer.h"

class DfxCacheTrack{
private:
	bool _active;
	std::string _label;
	std::string _path;
	int _offset;

	xvault::File *_file;
	DfxCacheRecorder *_cacheRecorder;
	DfxCachePlayer *_cachePlayer;
public:

	DfxCacheTrack();
	virtual ~DfxCacheTrack();

	int offset();
	void setOffset(int);

	bool isActive();
	void setActive(bool val);

	std::string label();
	void setLabel(std::string lbl);

	bool emptyPath();
	std::string path();
	void setPath(std::string pth);

	bool beginRecordingCache(int startingFrame = 0);
	void recordFrame();
	void endRecordingCache();

	void takeSnapshot(int frame);
	DfxCacheSnapshot *snapshot();

	void clearAll();
};