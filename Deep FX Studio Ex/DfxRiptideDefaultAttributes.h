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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <glm/vec3.hpp>

class DfxRiptideDefaultAttributes {
public:
	bool success;
	glm::vec3 *velocity;
	int *sph_class;
	int *alive;
	int *update;
	int *use_viscosity;
	float *viscosity;
	int *use_vorticity;
	float *vorticity;
	int *use_tension;
	float *tension;
	int *frozen;
};