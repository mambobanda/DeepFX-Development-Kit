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

#include <vector>
#include "DfxFluentSet.h"
#include "DfxFluentChannelMap.h"
#include "DfxFluentSimplexObject.h"

class DfxCacheSnapshot {
private:
	std::vector<DfxFluentSet*> _sets;
	std::vector<DfxFluentSimplexObject*> _simplexObjects;
	DfxFluentChannelMap _simplexAttributes;
	DfxFluentChannelMap _streamAttributes;

public:
	DfxCacheSnapshot();
	virtual ~DfxCacheSnapshot();

	std::vector<DfxFluentSet*> &sets();
	std::vector<DfxFluentSimplexObject*> &simplexObjects();
	DfxFluentChannelMap &simplexAttributes();
	DfxFluentChannelMap &streamAttributes();

	void clearSimplexAttributes();
	void clearStreamAttributes();
	void clearSimplexObjects();
	void clearSets();
	void clearAll();
};