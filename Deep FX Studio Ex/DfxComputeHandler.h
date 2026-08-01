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

#include <Fluent.h>
#include <DfxGlobal.h>
#include <FlFlow.h>
#include "DfxSpace.h"


class DfxComputeHandler {
protected:
	fl::Any *_any;
public:

	DfxComputeHandler();
	~DfxComputeHandler();

	bool fetch(std::string path);

	fl::Any *any();

	bool create(std::string label);
	bool destroy();

	static bool clearAllComputationGraphs();
	static std::vector<DfxComputeHandler> fetchAllFlowHandlers();
};