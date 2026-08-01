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

#include <glm\vec3.hpp>
#include <sstream>
#include <unordered_map>

class DfxObjectDisplacement {
private:
	glm::vec3 *position;
	size_t count;
	bool cached;
public:
	DfxObjectDisplacement();

	void clear();
	void init(size_t count);

	glm::vec3 *getPositions();
	size_t getCount();
	bool isCached();
	void setIsCached(bool);
};

class DfxObjectDisplacementMap {
private:
	std::unordered_map<std::string, DfxObjectDisplacement*> displacements;
public:
	DfxObjectDisplacementMap();
	~DfxObjectDisplacementMap();

	size_t getCount();
	void insertDisplacement(size_t index, DfxObjectDisplacement *);

	DfxObjectDisplacement* getDisplacement(size_t index);

	std::string indexToKey(size_t index);

	void clear();
};

