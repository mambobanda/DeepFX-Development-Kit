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

#include <iostream>
#include <string>
#include "DeepStudioRegistry.h"

namespace registry {
	const std::string _$APP_CURRENT_SCENE_PATH = "registry/scene/path";

	const std::string _$APP_SETTINGS_DIR = "registry/app/settings";
	const std::string _$APP_TEMP_DIR = "registry/app/temp";
	const std::string _$PYTHON_TOOL_DIR = "registry/app/python/tools";
	const std::string _$PYTHON_NODE_DIR = "registry/app/python/node";

	void initRegistry();

	void printRegistry();
}
