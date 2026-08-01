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
#include "RtxTimestepMethod.h"

namespace rtx {
	typedef struct _simulationConfig {
		// Boundary Method
		uint8_t boundaryMethod;

		// Search Type
		uint8_t searchMethod;

		// Timestep info
		TimestepMethod timestepMethod;
		float timestep;
		float min_timestep;
		float max_timestep;
		int fps;
		int substeps;

		// Gravity
		glm::vec3 gravity;

		// Corrections
		float density_error;
		float coherence_error;

		// Pressure Stiffness
		float pressure_stiffness;

		// Velocity Damping
		float velocity_dampening;

	}SimulationConfig;
}