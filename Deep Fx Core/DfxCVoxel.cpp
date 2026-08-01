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

#include <iostream>
#include <unordered_map>
#include "DfxCVoxel.h"


namespace dfxcore {

	Voxel::Voxel() {
		userdata = NULL;
	}

	Voxel::~Voxel() {
		freeUserData();
	}

	void Voxel::freeUserData() {
		if (userdata != NULL)
			delete userdata;
	}

	VoxelGrid::VoxelGrid(int size) {
		compact_hash = std::unordered_map<glm::vec3, Voxel*, SpatialHash>(size);

		gridCellSize = 0.0f;
		gridXCellCount = 0;
		gridYCellCount = 0;
		gridZCellCount = 0;
	}

	VoxelGrid::~VoxelGrid() {
	}

	void VoxelGrid::release() {
		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++)
			delete it->second;
	}

	DfxBounds &VoxelGrid::getBBox() {
		return bbox;
	}

	void VoxelGrid::setBBox(DfxBounds _bbox) {
		bbox = _bbox;
	}

	Voxel *VoxelGrid::getVoxel(glm::vec3 coord) {
		glm::vec3 location = getSpatialLocation(coord, bbox.xmin, bbox.ymin, bbox.zmin);
		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(location);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel *VoxelGrid::getDirectVoxel(glm::vec3 coord) {

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(coord);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}


	void VoxelGrid::resetSpatialHashAttributes() {
		setSpatialHashAttributes(gridCellSize, gridXCellCount * gridYCellCount * gridZCellCount);
	}

	glm::vec3 VoxelGrid::getLocation(glm::vec3 coord) {
		return  getSpatialLocation(coord, bbox.xmin, bbox.ymin, bbox.zmin);
	}

	void VoxelGrid::buildVoxelGrid(float size, float padding) {

		float halfSize = 0.5f*size;

		bbox.addPadding(padding, padding, padding);

		float xlength = std::abs((bbox.xmax + size) - (bbox.xmin - size));
		float ylength = std::abs((bbox.ymax + size) - (bbox.ymin - size));
		float zlength = std::abs((bbox.zmax + size) - (bbox.zmin - size));

		int xcellCount = (int)(xlength / (size));
		int ycellCount = (int)(ylength / (size));
		int zcellCount = (int)(zlength / (size));

		float xincrement = bbox.xmin;
		float yincrement = bbox.ymin;
		float zincrement = bbox.zmin;

		gridCellSize = size;
		gridXCellCount = xcellCount;
		gridYCellCount = ycellCount;
		gridZCellCount = zcellCount;

		resetSpatialHashAttributes();

		for (int i = 0; i <= xcellCount; i++) {

			float x = i;

			for (int j = 0; j <= ycellCount; j++) {

				float y = j;

				for (int k = 0; k <= zcellCount; k++) {

					float z = k;

					Voxel *vox = new Voxel();
					vox->active = false;

					vox->origin = glm::vec3(xincrement, yincrement, zincrement);
					vox->center = glm::vec3(xincrement + halfSize,
						yincrement + halfSize,
						zincrement + halfSize);

					vox->bounds.empty();
					vox->bounds.add(xincrement,
						yincrement,
						zincrement);

					vox->bounds.add(xincrement,
						yincrement,
						zincrement + size);

					vox->bounds.add(xincrement + size,
						yincrement,
						zincrement);

					vox->bounds.add(xincrement,
						yincrement + size,
						zincrement);

					vox->bounds.add(xincrement + size,
						yincrement,
						zincrement + size);

					vox->bounds.add(xincrement + size,
						yincrement + size,
						zincrement + size);

					vox->bounds.add(xincrement,
						yincrement + size,
						zincrement + size);

					vox->bounds.add(xincrement + size,
						yincrement + size,
						zincrement);

					glm::vec3 gridLocation(x, y, z);
					compact_hash[gridLocation] = vox;
					vox->gridLocation = gridLocation;

					zincrement += size;
				}

				zincrement = bbox.zmin;
				yincrement += size;
			}

			zincrement = bbox.zmin;
			yincrement = bbox.ymin;
			xincrement += size;
		}
	}

	std::vector<glm::vec3> VoxelGrid::getActiveVoxelLocations() {
		std::vector<glm::vec3> pts;

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			if (it->second->active)
				pts.push_back(it->second->origin);

		}

		return pts;
	}

	std::vector<glm::vec3> VoxelGrid::getInActiveVoxelLocations() {
		std::vector<glm::vec3> pts;

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			if (!it->second->active)
				pts.push_back(it->second->origin);

		}

		return pts;
	}

	void VoxelGrid::setVoxelActive(bool state) {

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			it->second->active = state;
		}
	}

	std::vector<glm::vec3> VoxelGrid::getActiveVoxelCenters() {
		std::vector<glm::vec3> pts;

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			if (it->second->active)
				pts.push_back(it->second->center);

		}

		return pts;
	}

	std::vector<Voxel*> VoxelGrid::getActiveVoxels() {
		std::vector<Voxel*> vxs;

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			if (it->second->active)
				vxs.push_back(it->second);
		}

		return vxs;
	}

	std::vector<Voxel*> VoxelGrid::getInActiveVoxels() {

		std::vector<Voxel*> vxs;

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.begin();
		it = compact_hash.begin();
		for (; it != compact_hash.end(); it++) {
			if (!it->second->active)
				vxs.push_back(it->second);
		}

		return vxs;
	}

	Voxel* VoxelGrid::getVoxelCorner0(Voxel* gl) {

		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y, gl->gridLocation.z - 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner1(Voxel* gl) {

		glm::vec3 key(gl->gridLocation.x + 1, gl->gridLocation.y, gl->gridLocation.z - 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner2(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x + 1, gl->gridLocation.y, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner3(Voxel* gl) {
		return gl;
	}

	Voxel* VoxelGrid::getVoxelCorner4(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y + 1, gl->gridLocation.z - 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner5(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x + 1, gl->gridLocation.y + 1, gl->gridLocation.z - 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner6(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x + 1, gl->gridLocation.y + 1, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getVoxelCorner7(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y + 1, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getLeftVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x - 1, gl->gridLocation.y, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getRightVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x + 1, gl->gridLocation.y, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getTopVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y + 1, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getBottomVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y - 1, gl->gridLocation.z);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getFrontVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y, gl->gridLocation.z - 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	Voxel* VoxelGrid::getBackVoxel(Voxel* gl) {
		glm::vec3 key(gl->gridLocation.x, gl->gridLocation.y, gl->gridLocation.z + 1);

		std::unordered_map<glm::vec3, Voxel*, SpatialHash>::iterator it = compact_hash.find(key);

		if (it != compact_hash.end()) {
			return it->second;;
		}
		else
			return NULL;
	}

	int VoxelGrid::getGridXCellCount() {
		return gridXCellCount;
	}

	int VoxelGrid::getGridYCellCount() {
		return gridYCellCount;
	}

	int VoxelGrid::getGridZCellCount() {
		return gridZCellCount;
	}
}