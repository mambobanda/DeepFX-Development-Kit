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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "RtxHeater.h"

namespace rtx {

	Heater::Heater() {

	}

	Heater::~Heater() {

	}

	int Heater::index() {
		return _id;
	}

	void Heater::setIndex(int val) {
		_id = val;
	}

	float Heater::radius() {
		return _radius;
	}

	void Heater::setRadius(float val) {
		_radius = val;
	}

	glm::vec3 Heater::position() {
		return _position;
	}

	void Heater::setPosition(glm::vec3 val) {
		_position = val;
	}

	void Heater::heat(size_t nbParticles, int8_t *alive, int8_t *frozen, int8_t *type, glm::vec3 *positions, float *viscosity) {
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {

				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				if(glm::distance(_position, positions[i]) < _radius) {
					frozen[i] = 0;
				}
			}
		}
	}
	
}