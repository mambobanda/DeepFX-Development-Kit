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

#include <DfxGlobal.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <VFile.h>
#include <VH5Buffer.h>
#include <VH5DocumentResource.h>
#include <VH5BufferResource.h>
#include <VH5Attribute.h>
#include <RJDocument.h>
#include <RJNode.h>
#include "DfxCacheFormat.h"
#include "DfxUtil.h"
#include "DfxCacheSimplexInfo.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentStreamCollection.h"

class DfxCacheRecorder {
private:
	size_t ID_GEN;
	xvault::File *_file;

	std::vector<DfxCacheSimplexInfo> simplexInfo;
	std::vector<std::string> simplexAttributePaths;
	std::vector<std::string> streamAttributePaths;
private:

	void saveSimplexResource(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info);

	void resetVertexBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update);
	void resetVertexNormalBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update);
	void resetUvBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update);
	void resetMaterialIDBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update);
	void resetIndexBufferInfo(DfxFluentSimplexObject *simObj, DfxCacheSimplexInfo &info, bool update);

	std::vector<std::string> saveAttributes(DfxFluentChannelMap &map, std::string pathPrefix);
	int stepCounter;
public:
	int startingFrame;
	size_t simplexSizeTracker;

public:
	
	DfxCacheRecorder(xvault::File *file);
	virtual ~DfxCacheRecorder();

	void step();
	void stepSimplexObjects();
	void stepSimplexAttributes();
	void stepStreamAttributes();
	void stepMetadata();
	void finalize();
};