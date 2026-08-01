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
#include <DfxRenderSessionInterface.h>

using DfxRenderSessionInstance = DfxRenderSessionInterface *(*)();

class DfxRenderManager {
private:
	static DfxRenderManager *_instance;
	std::unordered_map<unsigned int, DfxRenderSessionInstance> _instance_map;

	DfxRenderManager();
public:

	static DfxRenderManager *getInstance();

	void addRenderSessionCallback(unsigned int id, DfxRenderSessionInstance cb);
	void removeRenderSessionCallback(unsigned int id);

	DfxRenderSessionInterface *newRenderSession(unsigned int id);
};