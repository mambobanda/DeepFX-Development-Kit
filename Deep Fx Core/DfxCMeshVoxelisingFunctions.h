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

#include <DfxBounds.h>
#include <Dfx3dMeshData.h>
#include <vector>
#include <DfxUtility.h>
#include <DfxGlobal.h>
#include <DfxBounds.h>
#include <DfxCVoxel.h>
#include <glm\geometric.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <unordered_map.h>
#endif

namespace dfxcore {
	typedef struct {
		std::vector<int> t;
	}VoxTriangles;


	void resetSpatialHashAttributes(DfxBounds, float size, float padding);
	Voxel genVoxel(glm::vec3, DfxBounds &, float size);
	void voxelizeMesh(Dfx3dMeshData &mesh, glm::vec3 scale, std::vector<glm::vec3> &positions, float voxelSize);
	void voxelizeMeshVolume(Dfx3dMeshData &mesh, glm::vec3 scale, std::vector<glm::vec3> &positions, float voxelSize, bool shell);
}