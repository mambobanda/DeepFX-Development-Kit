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
#include <unordered_map>
#include <string>
#include "DfxFluentSet.h"

class DfxFluentTracker {
private:
	static DfxFluentTracker *_instance;

	std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> _sceneNodeTracker;
	std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> _simplexNodeTracker;

	DfxFluentTracker();
public:
	virtual ~DfxFluentTracker();

	static DfxFluentTracker *getInstance();

	std::unordered_map<std::string, std::vector<std::pair<size_t, size_t>>> &sceneNodeTracker();
	std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> &simplexNodeTracker();

	void trackNode(std::string path, std::vector<std::pair<size_t,size_t>> indices);
	void untrackNode(std::string path, std::vector<std::pair<size_t, size_t>> indices);
	void trackSimplex(size_t id, std::vector<std::pair<size_t, size_t>> indices);
	void untrackSimplex(size_t id, std::vector<std::pair<size_t, size_t>> indices);


	void clear();
};