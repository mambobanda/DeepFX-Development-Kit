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

#include <unordered_map>
#include <string>
#include <vector>
#include "DfxEnvelope.h"

class DfxEnvelopeMap {
private:
	static DfxEnvelopeMap *instance;

private:
	std::unordered_map< size_t, std::vector<DfxEnvelope*>> envMap;

	DfxEnvelopeMap();
public:
	static DfxEnvelopeMap *getInstance();

	void addEnvelope(size_t, DfxEnvelope *);

	bool hasEnvelope(size_t, size_t);

	std::vector<DfxEnvelope*> findEnvelopes(size_t);

	DfxEnvelope* findEnvelope(size_t instanceid, size_t propertyid);

	void removeEnvelope(size_t, size_t);

	void removeAllEnvelopes(size_t);
};
