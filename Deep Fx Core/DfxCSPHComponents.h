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

#include <glm\geometric.hpp>
#include <algorithm>
#include <cmath>
#include "Util.h"
#include "DfxCSPHParticle.h"
#include "DfxCSPHGlobal.h"

namespace dfxcore {
	
	namespace sph {
		
		class SPHComponents {
		public:
			static void calc_densities(Particle **particles, int nbParticles, float *boundary_mass, float mass);
			static void calc_densities(Particle **particles, int nbParticles, float mass);
			static void calc_normals(Particle **particles, int nbParticles, float mass, float h, glm::vec3 *normals);
			static void calc_boundary_masses(Particle **particles, int nbParticles, float *boundary_mass, float rdensity);
			static void calc_alpha(Particle **particles, int nbParticles, float *boundary_masses, float *alpha, float dt, float mass);
			static void damp_velocities(Particle **particles, int nbParticles, glm::vec3 *vi, float coeff);
			static glm::vec3 dampVelocity(glm::vec3 vel, float coeff);
			static void advance_velocities(Particle **particles, int nbParticles, glm::vec3 *forces, float dt);
			static void advance_velocities(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *vi, float dt);
			static void advance_positions(Particle **particles, int nbParticles, glm::vec3 *forces, float dt);
			static void advance_positions(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *vi, float dt);
		};
	}
}