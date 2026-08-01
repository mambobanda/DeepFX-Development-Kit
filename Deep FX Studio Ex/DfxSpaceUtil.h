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

#include "DfxSpace.h"
#include "DfxObjectHandler.h"
#include "DfxLxCameraHandler.h"
#include "DfxLxPointLightHandler.h"
#include "DfxLxEnvironmentLightHandler.h"
#include "DfxLxSunSkyLightHandler.h"

namespace util {
	void clearAllSceneHandlers();
	bool areBindable(fl::Any *source, fl::Any *destination);
	bool areBindable(std::string source, std::string destination);
	fl::Any *duplicateState(fl::Any *any, bool forceArray = false, bool includeSpace = true);
	void pushArray(fl::Any *anyArray, std::vector<fl::Any *> values);
}