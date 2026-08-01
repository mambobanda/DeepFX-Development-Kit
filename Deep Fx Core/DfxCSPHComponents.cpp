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

#include "DfxCSPHComponents.h"


namespace dfxcore {

	namespace sph {

		void SPHComponents::calc_densities(Particle **particles, int nbParticles, float *boundary_mass, float mass) {
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					/*if(particles[i]->getType() != FLUID_PARTICLE)
					continue;*/

					float rho = 0.0f;
					float rho_boundary = 0.0f;

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						float w = Global::getInstance()->conduit().W(particles[i]->x - np->x);

						if (np->getType() == FLUID_PARTICLE) {
							rho += mass * w;
						}
						else {
							rho_boundary += (boundary_mass[np->getIndex()]) * w;
						}

						//particles[i]->nbs[j].w = w;
					}

					particles[i]->density = rho + rho_boundary;
				}
			}
		}

		void SPHComponents::calc_densities(Particle **particles, int nbParticles, float mass) {
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if(particles[i]->getType() != FLUID_PARTICLE)
					continue;

					float rho = 0.0f;
					float rho_boundary = 0.0f;

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						float w = Global::getInstance()->conduit().W(particles[i]->x - np->x);

						if (np->getType() == FLUID_PARTICLE) {
							rho += mass * w;
						}
						
					}

					particles[i]->density = rho + rho_boundary;
				}
			}
		}

		void SPHComponents::calc_normals(Particle **particles, int nbParticles, float mass, float h, glm::vec3 *normals) {
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					glm::vec3 normal;
					for (int j = 0; j < particles[i]->nbCount; j++) {
						sph::Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						normal += mass / np->density * Global::getInstance()->conduit().gradW(particles[i]->x - np->x);;
					}

					normal = h * normal;
					normals[i] = normal;
				}
			}
		}

		void SPHComponents::calc_boundary_masses(Particle **particles, int nbParticles, float *boundary_mass, float rdensity) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != BOUNDARY_PARTICLE &&
						particles[i]->getType() != MOVING_BOUNDARY_PARTICLE)
						continue;

					float _mass = 0.0f;

					for (int j = 0; j < particles[i]->nbCount; j++) {
						if (!particles[particles[i]->nbIndices[j]]->isAlive()) continue;

						if (particles[i]->getType() == BOUNDARY_PARTICLE ||
							particles[i]->getType() == MOVING_BOUNDARY_PARTICLE) {
							Particle *np = particles[particles[i]->nbIndices[j]];

							_mass += Global::getInstance()->conduit().W(particles[i]->x - np->x);
						}

					}

					if (_mass > 0)
						boundary_mass[i] = rdensity / _mass;

				}
			}
		}

		void SPHComponents::calc_alpha(Particle **particles, int nbParticles, float *boundary_masses, float *alpha, float dt, float mass ) {

			float dt2 = dt *dt;
			float invdt2 = 1.f / dt2;

			#pragma omp parallel default(shared)
			{
			#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					glm::vec3 gradrhoSum = glm::vec3(0.0f);
					float gradLenSum = 0.0f;
	

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						glm::vec3 grad = Global::getInstance()->conduit().gradW(particles[i]->x - np->x);

						if (np->getType() == FLUID_PARTICLE) {

							glm::vec3 a = mass * grad;
							float b = pow2(glm::length(a));

							gradrhoSum += a;
							gradLenSum += b;

						}
						else {
							glm::vec3  a = boundary_masses[np->getIndex()] * grad;
							float b = pow2(glm::length(a));

							gradrhoSum += a;
							gradLenSum += b;

						}
					}

					float gradrhoSumSqr = pow2(glm::length(gradrhoSum));
					float denom = gradrhoSumSqr + gradLenSum;

					//Check denomninator is not too small
					denom = std::max(min_e, denom);

					float tempAlpha = 1.f / (denom);

					alpha[i] = tempAlpha;

				}
			}
		}

		glm::vec3 SPHComponents::dampVelocity(glm::vec3 vel, float coeff) {
			glm::vec3 new_vel(0.f);

			new_vel = vel - (vel * coeff);

			float speed = glm::length(new_vel);

			if (speed > 1.0e-5)
				return new_vel;
			else
				return glm::vec3(0.f);
		}

		void SPHComponents::damp_velocities(Particle **particles, int nbParticles, glm::vec3 *vi, float coeff) {
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					particles[i]->v = dampVelocity(vi[i], coeff);
				}
			}
		}

		void SPHComponents::advance_velocities(Particle **particles, int nbParticles, glm::vec3 *forces, float dt) {
			// Advance velocities using none pressure forces
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					particles[i]->v = particles[i]->v + ((forces[i] * dt));

				}
			}
		}

		void SPHComponents::advance_velocities(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *vi, float dt) {
			// Advance velocities using none pressure forces
			#pragma omp parallel default(shared)
			{
			#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					vi[i] = particles[i]->v + ((forces[i] * dt));
				}
			}
		}

		void SPHComponents::advance_positions(Particle **particles, int nbParticles, glm::vec3 *forces, float dt) {
			// Update particle positions
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					//particles[i]->x += dt * vi[i];
					particles[i]->x += dt * particles[i]->v;
				}
			}
		}

		void SPHComponents::advance_positions(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *vi, float dt) {
			// Update particle positions
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					particles[i]->x += dt * vi[i];
				}
			}
		}
	}
}