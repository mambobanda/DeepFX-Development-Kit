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

#include <unordered_map>
#include <vector>
#include "DfxComputeNodeDescription.h"

class DfxComputeNodeDB {
private:
	static DfxComputeNodeDB *_instance;

	std::unordered_map<std::string, DfxComputeNodeDescription*> _computeNodeMap;

	DfxComputeNodeDB();
public:
	virtual ~DfxComputeNodeDB();

	static DfxComputeNodeDB *getInstance();

	void registerComputeNode(DfxComputeNodeDescription *);

	DfxComputeNodeDescription *findComputeNodeDescription(std::string);

	std::vector<DfxComputeNodeDescription*> findAll();
};