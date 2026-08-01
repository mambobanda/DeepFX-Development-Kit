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
#include <unordered_map>
#include <DfxRenderInterface.h>

class DfxRenderRegistry {
public:
	enum { RENDERER = 0x100 };
private:
	struct Prop {
		uint32_t type;
		DfxRenderInterface *itemPtr;
	};
private:

	static DfxRenderRegistry *instance;

	std::unordered_map<size_t, Prop> propMap;
	DfxRenderRegistry();

public:

	static DfxRenderRegistry *getIntstance();

	void addProp(size_t uid, DfxRenderInterface *ptr, uint32_t type);

	void removeProp(size_t uid);

	bool hasProp(size_t uid);

	std::vector<DfxRenderInterface*> findAllTypes(uint32_t type);

	DfxRenderInterface* find(size_t type);
};