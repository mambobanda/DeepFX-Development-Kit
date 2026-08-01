/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/
#pragma once

#include <sstream>
#include "DfxObjectLoaderService.h"

static DfxGlobalEntityName DfxObjLoaderPlugin_Identifier = "2F2X252X_DFX_OBJ_LOADER_PLUGIN";

class DfxObjLoaderPlugin: public DfxObjectLoaderService {
public:
	DfxObjLoaderPlugin(DfxGlobalEntityName uniquename);

	void loadObject(Dfx3dMeshData &mesh) override;

	static void initialize();
	static void *createService(DfxGlobalEntityName uniquename);
};