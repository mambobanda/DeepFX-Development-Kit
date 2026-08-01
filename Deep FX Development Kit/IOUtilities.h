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


#include <string>
#include <string>
#include <assimp\Exporter.hpp>
#include <assimp\scene.h>
#include "Dfx3dMeshData.h"

namespace IOUtilities {
	
	#define MESH_EXPORTER_ASSBIN_FORMAT 0
	#define MESH_EXPORTER_OBJ_FORMAT 1
	
	void meshDataToObj(std::string, Dfx3dMeshData * );
	
}