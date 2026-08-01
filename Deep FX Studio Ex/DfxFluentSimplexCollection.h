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

#include <mutex>
#include "DfxFluentChannelMap.h"
#include "DfxFluentSimplexObject.h"
#include "DfxFluentBigID.h"

class DfxFluentSimplexCollection {
private:
	static DfxFluentSimplexCollection *_instance;

	std::vector<DfxFluentSimplexObject*> _simplexObjects;
	DfxFluentChannelMap _attributeMap;

	std::mutex lock;

	DfxFluentSimplexCollection();
public:
	
	virtual ~DfxFluentSimplexCollection();

	static DfxFluentSimplexCollection *getInstance();

	size_t addObject(DfxFluentSimplexObject* obj, std::string groupid, std::string scene_path, glm::mat4 xform = glm::mat4(1.f));

	std::vector<DfxFluentSimplexObject*> &simplexObjects();
	DfxFluentChannelMap &atrributeMap();

	size_t size();

	void initAttributeMap();
	void clearAllObjects();
	void clearAttributeMap();

	bool immutableAttribute(std::string);
};