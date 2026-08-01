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
#include "DfxSpace.h"
#include "DfxEventEnums.h"
#include "DfxFlowHandler.h"

class DfxListenerHandler {
protected:
	fl::Any *_dataObject;
public:
	
	DfxListenerHandler();
	~DfxListenerHandler();

	bool fetch(std::string path);
	fl::Any *dataObject();

	bool create(unsigned int);
	bool destroy();

	bool removeFlowPath(std::string path);

	static bool clearAllListeners();
	static std::vector<DfxListenerHandler> fetchAllListenerHandlers();
	std::vector<DfxFlowHandler> fetchAllRelatedFlowGraphs();
};