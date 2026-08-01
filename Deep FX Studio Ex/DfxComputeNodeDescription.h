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

#include <functional>
#include <FlAny.h>
#include <FlSpace.h>

using ComputeNodeInstance = std::function<fl::Any*(fl::Space*)>;

class DfxComputeNodeDescription {
public:
	std::string identifier;
	std::string name;
	std::string description;
	std::string category;
	std::string placement;
	int major;
	int minor;
	ComputeNodeInstance instance;
};