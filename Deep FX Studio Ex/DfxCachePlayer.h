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

#include <filesystem>
#include <DfxGlobal.h>
#include <VH5File.h>
#include <VDocument.h>
#include <RJDocument.h>
#include <VH5Attribute.h>
#include <VH5Buffer.h>
#include <VH5IResource.h>
#include <VH5OResource.h>
#include "DfxCacheFormat.h"
#include "DfxCacheSnapShot.h"
#include "DfxCacheSimplexInfo.h"

namespace filesystem = std::experimental::filesystem;

class DfxCachePlayer {
private:
	DfxCacheSnapshot *_snapshot;
	std::string _currentPath;

private:
	size_t _simplexCollectionSize;
	std::vector<DfxFluentSimplexObject*> _simplexObjects;
	std::vector<DfxCacheSimplexInfo> _simplexObjectInfo;

private:
	void updateSimplexObject(xvault::File *_file, size_t index, xvault::Node *node);
	DfxFluentSet *retrieveSet(xvault::Node *node);
	void loadAttribute(xvault::File *ifile, DfxFluentChannelMap &map, std::string path);
public:
	DfxCachePlayer();
	virtual ~DfxCachePlayer();

	void takeSnapshot(std::string cachePath, int currentFrame, int offset);
	DfxCacheSnapshot *snapshot();

	void clear();
};