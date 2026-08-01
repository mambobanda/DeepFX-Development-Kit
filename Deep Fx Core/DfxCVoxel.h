/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2016 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#pragma once

#include <glm\vec3.hpp>
#include <vector>
#include <unordered_map>
#include <DfxBounds.h>
#include "DfxCSpatialHash.h"
//#include "UserData.h"


namespace dfxcore {

	class Voxel {
	public:
		Voxel();
		~Voxel();
		DfxBounds bounds;
		bool active;
		glm::vec3 origin;
		glm::vec3 center;
		glm::vec3 gridLocation;
		void *userdata;

		void freeUserData();
	};

	class VoxelGrid {
	private:
		std::unordered_map<glm::vec3, Voxel*, SpatialHash> compact_hash;
		DfxBounds bbox;
		float gridCellSize;
		int gridXCellCount;
		int gridYCellCount;
		int gridZCellCount;
	public:
		VoxelGrid(int size);

		~VoxelGrid();

		DfxBounds &getBBox();

		void setBBox(DfxBounds _bbox);

		Voxel *getVoxel(glm::vec3 coord);

		Voxel *getDirectVoxel(glm::vec3 coord);

		void resetSpatialHashAttributes();

		std::vector<glm::vec3> getActiveVoxelLocations();

		std::vector<glm::vec3> getInActiveVoxelLocations();

		std::vector<glm::vec3> getActiveVoxelCenters();

		void setVoxelActive(bool state);

		glm::vec3 getLocation(glm::vec3 coord);

		std::vector<Voxel*> VoxelGrid::getActiveVoxels();

		std::vector<Voxel*> VoxelGrid::getInActiveVoxels();

		void buildVoxelGrid(float cellsize, float padding);

	   /*template<typename T>
		void initUserData(NewInstance<T> &ni) {
			unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();

			it = compact_hash.begin();
			for (; it != compact_hash.end(); it++) {
				it->second->userdata = ni.instance();
			}
		}*/

		Voxel* getVoxelCorner0(Voxel* gridpoint);
		Voxel* getVoxelCorner1(Voxel* gridpoint);
		Voxel* getVoxelCorner2(Voxel* gridpoint);
		Voxel* getVoxelCorner3(Voxel* gridpoint);
		Voxel* getVoxelCorner4(Voxel* gridpoint);
		Voxel* getVoxelCorner5(Voxel* gridpoint);
		Voxel* getVoxelCorner6(Voxel* gridpoint);
		Voxel* getVoxelCorner7(Voxel* gridpoint);

		Voxel* getLeftVoxel(Voxel* gridpoint);
		Voxel* getRightVoxel(Voxel* gridpoint);
		Voxel* getTopVoxel(Voxel* gridpoint);
		Voxel* getBottomVoxel(Voxel* gridpoint);
		Voxel* getFrontVoxel(Voxel* gridpoint);
		Voxel* getBackVoxel(Voxel* gridpoint);

		int VoxelGrid::getGridXCellCount();

		int VoxelGrid::getGridYCellCount();

		int VoxelGrid::getGridZCellCount();

		void release();
	};

	//std::vector<Voxel> build_voxel_grid(float size, DfxBounds &bbox);
}