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
#include <vector>
#include <DfxGlobal.h>
#include "DfxSceneGraphMenuBar.h"
#include "DeepStudioRegistry.h"
#include "DfxPythonNodeLoader.h"
#include "DeepStudioRegistryPaths.h"

namespace filesystem = std::experimental::filesystem;

class DfxPythonNodeList {
private:
	static std::vector<DfxPythonNodeLoader::PyNodeDesc*> list;
public:

	static void add(DfxPythonNodeLoader::PyNodeDesc*);
	static void disable(size_t index);
	static void enable(size_t index);
	static size_t count();
	static DfxPythonNodeLoader::PyNodeDesc *getNodeInfo(size_t index);

	static void loadAll();
	static void installMenus(QMenu *menu);
};
