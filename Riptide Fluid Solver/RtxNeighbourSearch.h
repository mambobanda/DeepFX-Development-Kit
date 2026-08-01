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

#include <glm/vec3.hpp>
#include "RtxCommon.h"

class RtxNeighbourSearch {
public:

	virtual void findNeighbours(size_t nbParticles, int8_t nbListSize, float h, float radius, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours)= 0;
};