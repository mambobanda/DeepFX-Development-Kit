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

#include "DfxCacheSnapShot.h"


DfxCacheSnapshot::DfxCacheSnapshot() {

}

DfxCacheSnapshot::~DfxCacheSnapshot() {}

std::vector<DfxFluentSet*> &DfxCacheSnapshot::sets() {
	return _sets;
}

std::vector<DfxFluentSimplexObject*> &DfxCacheSnapshot::simplexObjects() {
	return _simplexObjects;
}

DfxFluentChannelMap &DfxCacheSnapshot::simplexAttributes() {
	return _simplexAttributes;
}

DfxFluentChannelMap &DfxCacheSnapshot::streamAttributes() {
	return _streamAttributes;
}

void DfxCacheSnapshot::clearSimplexAttributes() {
	_simplexAttributes.removeAllAttributes();
}

void DfxCacheSnapshot::clearStreamAttributes() {
	_streamAttributes.removeAllAttributes();
}

void DfxCacheSnapshot::clearSimplexObjects() {
	_simplexObjects.clear();
}

void DfxCacheSnapshot::clearSets() {
	for (auto set : _sets) delete set;
	_sets.clear();
}

void DfxCacheSnapshot::clearAll() {
	clearSimplexAttributes();
	clearStreamAttributes();
	clearSimplexObjects();
	clearSets();
}