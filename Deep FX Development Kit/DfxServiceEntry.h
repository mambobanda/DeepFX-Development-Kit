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
#include "DfxEntityCommon.h"

using DfxCreateService = void*(*)(DfxGlobalEntityName uniquename);

class DfxServiceEntry {
private:
	DfxGlobalEntityName uniquename;
	std::vector<std::pair<std::string, std::string>> tags;
	DfxCreateService createService;

public:
	
	DfxGlobalEntityName getUniqueName();
	void setUniqueName(DfxGlobalEntityName uniquename);

	unsigned int tagCount();
	std::pair<std::string, std::string> getTag(unsigned int);
	void addTag(std::string name, std::string content);
	void setCreate(DfxCreateService);
	void *newInstance();
};

