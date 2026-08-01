/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

namespace sct {

	inline void clear_forces(size_t nbParticles, glm::vec3 *forces) {
		for (int i = 0; i < nbParticles; i++) {
			forces[i] = glm::vec3(0.f, 0.f, 0.f);
		}
	}

	inline void calc_gravity(size_t nbParticles, glm::vec3 gravity, glm::vec3 *forces, int8_t *alive) {
		for (int i = 0; i < nbParticles; i++) {
			if (!alive[i]) continue;

			forces[i] += gravity;
		}
	}

};