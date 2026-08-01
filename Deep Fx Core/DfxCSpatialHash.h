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


#include <functional>
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>

namespace dfxcore {

	static float SP_HASH_CELL_SIZE = -1.0f;
	static size_t SP_HASH_CELL_COUNT = -1;

	inline void setSpatialHashAttributes(float cellsize, int cellcount) {
		SP_HASH_CELL_SIZE = cellsize;
		SP_HASH_CELL_COUNT = cellcount;
	}

	inline glm::vec3 getSpatialLocation(glm::vec3 p, float xmin, float ymin, float zmin) {
		return glm::vec3(std::floor((p.x - xmin) / SP_HASH_CELL_SIZE),
			std::floor((p.y - ymin) / SP_HASH_CELL_SIZE),
			std::floor((p.z - zmin) / SP_HASH_CELL_SIZE));
	}

	struct SpatialHash {

		size_t operator()(glm::vec3 const& p) const {
			size_t _x = (size_t)p.x * 73856093;
			size_t _y = (size_t)p.y * 19349663;
			size_t _z = (size_t)p.z * 83492791;

			return (_x ^ _y ^ _z) % SP_HASH_CELL_COUNT;
		}
	};
	
}