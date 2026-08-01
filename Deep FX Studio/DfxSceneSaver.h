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

#include <experimental\filesystem>
#include <fstream>
#include <DfxLayerManager.h>
#include "IOUtilities.h"
#include "DfxSceneWriterTool.h"

namespace filesystem = std::experimental::filesystem;

class DfxSceneSaver {
private:

public:
	DfxSceneSaver();

	void save(std::string path);
	void makeUnreferencedObjectsPersistent(std::string path);
};
