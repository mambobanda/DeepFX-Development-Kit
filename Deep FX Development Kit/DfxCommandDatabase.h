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

#include <DfxString.h>
#include <unordered_map>
#include <sstream>
#include "DfxGlobal.h"

using DfxCommandCreatePtr = void *(*)();

class DfxCommandDatabase {
private:
	static DfxCommandDatabase *instance;

	std::unordered_map<std::string, DfxCommandCreatePtr> db;

	DfxCommandDatabase();
public:

	static DfxCommandDatabase *getInstance();

	bool mapCommand(DfxString, DfxCommandCreatePtr);

	DfxCommandCreatePtr findCommand(DfxString);
};


