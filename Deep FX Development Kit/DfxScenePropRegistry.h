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
#include <DfxViewItem.h>

class DfxScenePropRegistry {
public:
	enum { CAMERA=0x100, LIGHT, };
private:
	struct Prop {
		uint32_t type;
		DfxViewItem *itemPtr;
	};
private:

	static DfxScenePropRegistry *instance;

	std::unordered_map<size_t, Prop> propMap;
	DfxScenePropRegistry();

public:

	static DfxScenePropRegistry *getIntstance();

	void addProp(size_t uid, DfxViewItem *ptr, uint32_t type);

	void removeProp(size_t uid);

	bool hasProp(size_t uid);

	std::vector<DfxViewItem*> findAllTypes(uint32_t type);
};