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
	
	inline void calc_intermediate_velocity_position(size_t nbParticles, float dt, glm::vec3 *velocity, glm::vec3 *position, glm::vec3 *position_i, int8_t *alive, int8_t *type, glm::vec3 *forces) {

		for (int i = 0; i < nbParticles; i++) {
			if (!alive[i]) continue;

			velocity[i] = velocity[i] + dt * forces[i];
			position_i[i] = position[i] + dt* velocity[i];
		}

	}

	inline void calc_final_velocity_position(size_t nbParticles, float dt, glm::vec3 *velocity, glm::vec3 *position, glm::vec3 *position_i, int8_t *alive, int8_t *type, glm::vec3 *forces,
		float vdampening = 1.f) {
		
		for (int i = 0; i < nbParticles; i++) {
			if (!alive[i]) continue;

			velocity[i] = position_i[i] - position[i];
			velocity[i] -= velocity[i] * vdampening;
			position[i] = position_i[i];
		}

	}
}