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
#include <glm/geometric.hpp>
#include <algorithm>
#include "RtxCommon.h"

// int maxIterations = -std::numeric_limits<int>::infinity();

namespace rtx {

	// calc boundary masses
	template<typename T>
	void calc_boundary_masses(T &kernel, size_t nbParticles, int8_t nbListSize, float rdensity, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours, float *boundary_mass) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!alive[i]) continue;

					if (type[i] != BOUNDARY_PARTICLE &&
						type[i] != MOVING_BOUNDARY_PARTICLE)
						continue;

					float _mass = 0.0f;

					auto nbListOffset = i * nbListSize;
					for (int j = 0; j < nbCount[i]; j++) {

						if (!alive[neighbours[nbListOffset + j]]) continue;

						if (type[neighbours[nbListOffset + j]] == BOUNDARY_PARTICLE ||
							type[neighbours[nbListOffset + j]] == MOVING_BOUNDARY_PARTICLE) {

							glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
							_mass += kernel.W(r);
						}

					}

					if (_mass > 0)
						boundary_mass[i] = (rdensity / _mass);

				}
			}
	}

	// calculate Density
	template<typename T>
	void calc_densities(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *density) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!alive[i]) continue;

					if (type[i] != FLUID_PARTICLE)
						continue;

					float rho = 0.0f;
					float rho_boundary = 0.0f;

					auto nbListOffset = i * nbListSize;
					for (int j = 0; j < nbCount[i]; j++) {

						if (!alive[neighbours[nbListOffset + j]]) continue;

						glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
						float w = kernel.W(r);

						if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
							rho += mass * w;
						}
						else if (boundaryMethod == AKINCI2012) {
							rho_boundary += (boundary_mass[neighbours[nbListOffset + j]]) * w;
						}
						else if (boundaryMethod == STATIC_PARTICLES) {
							rho_boundary += mass * w;
						}

					}

					density[i] = rho + rho_boundary;
				}
			}
	}

	// calculate Stiffness Coefficient
	template<typename T>
	void calc_alpha(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *alpha) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!alive[i]) continue;

					if (type[i] != FLUID_PARTICLE)
						continue;

					glm::vec3 gradrhoSum = glm::vec3(0.0f);
					float gradLenSum = 0.0f;

					auto nbListOffset = i * nbListSize;
					for (int j = 0; j < nbCount[i]; j++) {

						if (!alive[neighbours[nbListOffset + j]]) continue;

						glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
						glm::vec3 grad = kernel.gradW(r);


						if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {

							glm::vec3 a = mass * grad;
							float b = pow2(glm::length(a));

							gradrhoSum += a;
							gradLenSum += b;

						}
						else if (boundaryMethod == AKINCI2012) {
							glm::vec3  a = boundary_mass[neighbours[nbListOffset + j]] * grad;
							float b = pow2(glm::length(a));

							gradrhoSum += a;
							gradLenSum += b;

						}
						else if (boundaryMethod == STATIC_PARTICLES) {
							glm::vec3  a = mass * grad;
							float b = pow2(glm::length(a));

							gradrhoSum += a;
							gradLenSum += b;

						}

					}

					float gradrhoSumSqr = pow2(glm::length(gradrhoSum));
					float denom = gradrhoSumSqr + gradLenSum;

					//Check denomninator is not too small
					denom = std::max((float)RTX_MIN_E, denom);

					float tempAlpha = 1.f / (denom);

					alpha[i] = tempAlpha;

				}
			}
	}

	template<typename T>
	void calc_vorticity_confinement(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, glm::vec3 *positions, glm::vec3 *velocity,
		int8_t *alive, int8_t *type, int8_t *nbCount, size_t *neighbours, float *density, glm::vec3 *forces, int8_t *useVorticity, float *vorticity) {

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)  
			for (int i = 0; i < (int)nbParticles; i++) {
				if (!useVorticity[i]) continue;

				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				glm::vec3 vorticityForce(0.0f);

				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {

					if (!alive[neighbours[nbListOffset + j]]) continue;

					if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
						glm::vec3 vij = velocity[neighbours[nbListOffset + j]] - velocity[i];
						glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
						glm::vec3 w = vij * kernel.gradW(r);
						float _w = glm::length(w);
						glm::vec3 p = ((mass * positions[i]) + (mass * positions[neighbours[nbListOffset + j]])) / (mass + mass);
						glm::vec3 n = p - positions[i];
						float _n = (glm::length(n));

						if (_n != 0 && _w != 0) {
							glm::vec3 N = n / _n;
							vorticityForce += vorticity[i] * ((w / _w) * n);
						}
					}
				}
			
				forces[i] += checkVec3Bounds(vorticityForce  *  mass);

			}
		}
	}

	template<typename T>
	void calc_surface_tension(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, float pradius, float rdensity, glm::vec3 *positions, glm::vec3 *velocity,
		int8_t *alive, int8_t *type, int8_t *nbCount, size_t *neighbours, float *density, glm::vec3 *forces, float *boundary_mass, int8_t *useSurfaceTension, float *surfaceTension) {

		// const Real k = m_surfaceTension;
		//const Real radius = sim->getValue<Real>(Simulation::PARTICLE_RADIUS);
		const float diameter = 2.f * pradius;
		const float diameter2 = diameter * diameter;

		// Compute forces
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)  
			for (int i = 0; i < (int)nbParticles; i++) {
				if (!useSurfaceTension[i]) continue;

				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				const glm::vec3 xi = positions[i];
				glm::vec3 tension(0.f);

				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {
					if (!alive[neighbours[nbListOffset + j]]) continue;

					if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
						const glm::vec3 xixj = positions[i] - positions[neighbours[nbListOffset + j]];
						const float r2 = glm::dot(xixj, xixj);
						glm::vec3 vxvj = velocity[i] - velocity[neighbours[nbListOffset + j]];


						if (r2 > diameter2)
							tension -= surfaceTension[i] / mass * mass * (xixj) * kernel.W(xixj);
						else
							tension -= surfaceTension[i] / mass * mass * (xixj) * kernel.W(glm::vec3(diameter, 0.0, 0.0));
					}
					else if (boundaryMethod == AKINCI2012) {
						/*if (!useSurfaceTension[neighbours[nbListOffset + j]]) continue;

						const glm::vec3 xixj = positions[i] - positions[neighbours[nbListOffset + j]];
						const float r2 = glm::dot(xixj, xixj);
						glm::vec3 a;
						if (r2 > diameter2)
							a = -surfaceTension[neighbours[nbListOffset + j]] / mass * rdensity * boundary_mass[neighbours[nbListOffset + j]] * (xixj) * kernel.W(xixj);
						else
							a = -surfaceTension[neighbours[nbListOffset + j]] / mass * rdensity * boundary_mass[neighbours[nbListOffset + j]] * (xixj) * kernel.W(glm::vec3(diameter, 0.0, 0.0));
						tension += a;
						*/
					}			
				}

				forces[i] += tension;
			}
		}
	}

	template<typename T>
	void calc_viscosity(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, float h, int8_t *use_viscosity, float *viscosity,
		glm::vec3 *positions, glm::vec3 *velocity, int8_t *alive, int8_t *type, int8_t *nbCount, size_t *neighbours, float *boundary_mass, glm::vec3 *forces, float *density) {

		float h2 = h * h;
		float invH = 1.f / h;

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {

				if (!use_viscosity[i]) continue;

				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				glm::vec3 visco(0.f);
				glm::vec3 viscoBoundary(0.f);
				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {

					if (!alive[neighbours[nbListOffset + j]]) continue;

					glm::vec3 xixj = positions[i] - positions[neighbours[nbListOffset + j]];
					glm::vec3 gradw = kernel.gradW(xixj);
					glm::vec3 vxvj = velocity[neighbours[nbListOffset + j]] - velocity[i];
		
					if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
					
						visco += (mass / density[neighbours[nbListOffset + j]]) * (vxvj) * kernel.W(xixj);
					}
					else if (boundaryMethod == AKINCI2012) {
						if(!use_viscosity[neighbours[nbListOffset + j]]) continue;
						glm::vec3 xixj = positions[i] - positions[neighbours[nbListOffset + j]];
						glm::vec3 vxvj = velocity[i] - velocity[neighbours[nbListOffset + j]];
						glm::vec3 a = -invH * viscosity[neighbours[nbListOffset + j]] * (1000.f * boundary_mass[neighbours[nbListOffset + j]] / density[i]) * (vxvj)* kernel.W(xixj);
						forces[i] += a;
						
					}
					else if (boundaryMethod == STATIC_PARTICLES) {
						glm::vec3 ai = 2.0f * viscosity[neighbours[nbListOffset + j]] * (mass / density[i]) * vxvj * (glm::dot(xixj, gradw)) / (pow2(glm::length(xixj) + 0.01f*h2));
						forces[i] += checkVec3Bounds(ai);
					}
				}
		
				velocity[i] += viscosity[i] * visco;
			}
		}
	}

	// Calc Gravity
	inline void calc_gravity(size_t nbParticles, glm::vec3 gravity, int8_t *alive, int8_t *type, glm::vec3 *forces) {

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				forces[i] = (gravity);
			}
		}
	}

	// Freeze Particles
	inline void freeze(size_t nbParticles, int8_t *alive, int8_t *frozen, int8_t *type, glm::vec3 *forces) {

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {

				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				if (frozen[i] == 1) {
					forces[i] = glm::vec3(0);
				}
			}
		}
	}

	// Advance Velocity
	inline void advance_velocity(size_t nbParticles, float dt, glm::vec3 *velocity, glm::vec3 *velocity_i, int8_t *alive, int8_t *type, glm::vec3 *forces) {
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				velocity_i[i] = velocity[i] + ((forces[i] * dt));

			}
		}
	}

	inline void calc_position(size_t nbParticles, float dt, int8_t *frozen, glm::vec3 *positions, glm::vec3 *velocity_i, int8_t *alive, int8_t *type) {
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE || frozen[i] == 1)
					continue;

				positions[i] += dt * velocity_i[i];
			}
		}
	}

	inline void damp_velocity(size_t nbParticles, float velocity_dampening, int8_t *frozen, glm::vec3 *velocity, glm::vec3 *velocity_i, int8_t *alive, int8_t *type) {

		// Correct Velocities
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)	
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE) continue;

				glm::vec3 new_vel(0.f);

				new_vel = velocity_i[i] - (velocity_i[i] * velocity_dampening);

				float speed = glm::length(new_vel);

				if (speed <= 1.0e-5) new_vel = glm::vec3(0.f);
				
				if (frozen[i] != 1) {
					velocity[i] = velocity_i[i];
				}
				else {
					velocity[i] = glm::vec3(0.f);
				}
			}
		}
	}

	// Calculate density change
	template<typename T>
	void calc_density_change(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, glm::vec3 *positions, glm::vec3 *velocity_i, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *density_gradient) {

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				float rhograd(0.0f);

				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {

					if (!alive[neighbours[nbListOffset + j]]) continue;

					glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
					glm::vec3 grad = kernel.gradW(r);

					glm::vec3 vij = velocity_i[i] - velocity_i[neighbours[nbListOffset + j]];

					if (type[i] == FLUID_PARTICLE) {
						rhograd += mass * glm::dot(vij, grad);
					}
					else if (boundaryMethod == AKINCI2012) {
						rhograd += boundary_mass[neighbours[nbListOffset + j]] * glm::dot(vij, grad);
					}
					else if (boundaryMethod == STATIC_PARTICLES) {
						rhograd += mass * glm::dot(vij, grad);
					}

				}

				float densityAdv = std::max(densityAdv, 0.0f);

				if (nbCount[i] < 20)
					densityAdv = 0.0;

				density_gradient[i] = densityAdv;
			}
		}
	}

	// Calc Density advance
	template<typename T>
	void calc_density_advance(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, float dt, float rdensity, float h, glm::vec3 *positions, glm::vec3 *velocity_i, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *density, float *density_i) {

		//Calculate Density Rate of change
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				float rhoadv(0.0f);

				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {

					if (!alive[neighbours[nbListOffset + j]]) continue;

					glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
					glm::vec3 grad = kernel.gradW(r);

					glm::vec3 vij = velocity_i[i] - velocity_i[neighbours[nbListOffset + j]];

					if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE)
						rhoadv += mass * glm::dot(grad, vij);
					else if(boundaryMethod == AKINCI2012)
						rhoadv += boundary_mass[neighbours[nbListOffset + j]] * glm::dot(grad, vij);
					else if (boundaryMethod == STATIC_PARTICLES)
						rhoadv += mass * glm::dot(grad, vij);

				}

				auto advCheck = density[i] + dt * rhoadv;

				density_i[i] = density[i] + dt * rhoadv;
				density_i[i] = std::max(density_i[i], rdensity);

			}
		}

	}

	// Correct Density Error
	template<typename T>
	void correct_density_error(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, float rdensity, float densityError, float dt, float h, float pressure_stiffness,
		glm::vec3 *positions, glm::vec3 *velocity_i, int8_t *alive, int8_t *type, int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *density,
		float *density_i, float *density_gradient, float *alpha, glm::vec3 *pressureForce, float *ki00) {

		int iter = 0;
		//_density_solver_iter = iter;
		float delta = 0.01f * rdensity;
		float avgComp = 0.0f;
		float eta = densityError * 0.01f * rdensity;  // maxError is given in percent
		int   pcounter = 0;
		double error = 1e100;

		float _dt2 = dt * dt;
		float _invdt2 = 1.f / _dt2;
		float h2 = h * h;

		calc_density_advance(kernel, boundaryMethod, nbParticles, nbListSize, mass, dt, rdensity, h, positions, velocity_i, alive, type, nbCount, neighbours, boundary_mass, density, density_i);

		// Warm Start
		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				float ki_rho = ki00[i];

				glm::vec3 boundaryPressureForce(0.0f);

				auto nbListOffset = i * nbListSize;
				for (int j = 0; j < nbCount[i]; j++) {

					if (!alive[neighbours[nbListOffset + j]]) continue;

					glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
					glm::vec3 grad = kernel.gradW(r);

					if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
						glm::vec3 _pressureForce(0.0f);

						float diff2 = std::max(0.f, density_i[neighbours[nbListOffset + j]] - rdensity);
						float kj = diff2 * _invdt2 *  alpha[neighbours[nbListOffset + j]];
						float kj_rho = kj;

						float ksum = (ki_rho + kj_rho);

						if (ksum > (float)RTX_MIN_E) {
							_pressureForce = pressure_stiffness * mass * (ksum)* grad;
							velocity_i[i] -= dt * _pressureForce;
						}

					}
					else if (boundaryMethod == AKINCI2012) {
						if (ki_rho > (float)RTX_MIN_E) {
							glm::vec3 _pressureForce(0.0f);

							_pressureForce = pressure_stiffness * boundary_mass[neighbours[nbListOffset + j]] * (ki_rho)* grad;
							velocity_i[i] += -dt * _pressureForce;
						}
					}
					else if (boundaryMethod == STATIC_PARTICLES) {
						if (ki_rho > (float)RTX_MIN_E) {
							glm::vec3 _pressureForce(0.0f);

							_pressureForce = pressure_stiffness * mass * (ki_rho)* grad;
							velocity_i[i] += -dt * _pressureForce;
						}
					}
				}

				ki00[i] = 0.f;
			}
		}
		//////////////////////////////////////////////////////////////

		// Correct Density
		while (((error > eta) || (iter < 2)) && (iter < 50)) {
			//	for(int adv=0; adv < 10; adv++){
			if (iter > 50) break;

			//Reset Compression Error Variables
			pcounter = 0;
			error = 0.0f;
			avgComp = 0.0f;

			////////////////////Adapt Velocities////////////////////////
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!alive[i]) continue;

					if (type[i] != FLUID_PARTICLE)
						continue;

					float diff = std::max(0.f, density_i[i] - rdensity);
					float ki = diff * _invdt2 * alpha[i];
					float ki_rho = ki;

					ki00[i] += ki;

					glm::vec3 boundaryPressureForce(0.0f);

					auto nbListOffset = i * nbListSize;
					for (int j = 0; j < nbCount[i]; j++) {

						if (!alive[neighbours[nbListOffset + j]]) continue;

						glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
						glm::vec3 grad = kernel.gradW(r);

						if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
							glm::vec3 _pressureForce(0.0f);				

							float diff2 = std::max(0.f, density_i[neighbours[nbListOffset + j]] - rdensity);
							float kj = diff2 * _invdt2 *  alpha[neighbours[nbListOffset + j]];
							float kj_rho = kj;

							float ksum = (ki_rho + kj_rho);

							if (ksum > (float)RTX_MIN_E) {
								_pressureForce = pressure_stiffness * mass * (ksum) * grad;
								velocity_i[i] -= dt * _pressureForce;
							}

						}
						else if (boundaryMethod == AKINCI2012) {
							if (ki_rho > (float)RTX_MIN_E) {
								glm::vec3 _pressureForce(0.0f);

								_pressureForce = pressure_stiffness * boundary_mass[neighbours[nbListOffset + j]] * (ki_rho)* grad;
								boundaryPressureForce += _pressureForce;

								velocity_i[i] += -dt * _pressureForce;
							}
						}
						else if (boundaryMethod == STATIC_PARTICLES) {
							if (ki_rho > (float)RTX_MIN_E) {
								glm::vec3 _pressureForce(0.0f);

								_pressureForce = pressure_stiffness * mass * (ki_rho)* grad;
								boundaryPressureForce += _pressureForce;

								velocity_i[i] += -dt * _pressureForce;
							}
						}
					}

					pressureForce[i] += boundaryPressureForce;

				}
			}

			calc_density_advance(kernel, boundaryMethod, nbParticles, nbListSize, mass, dt, rdensity, h, positions, velocity_i, alive, type, nbCount, neighbours, boundary_mass, density, density_i);

			//#pragma omp for reduction(+:avgComp) schedule(static)
			for (int i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				float temp_avg = density_i[i] - rdensity;
				avgComp += temp_avg;

				++pcounter;
			}

			error = avgComp / pcounter;

			iter++;

		}	
		
		if(iter > 0) for (int i = 0; i < nbParticles; i++) ki00[i] *= h2;

		//maxIterations = std::max(maxIterations, iter);
		//std::cout << "Max Iterations: " << maxIterations << std::endl;
		//std::cout << "Iterations: " << iter << std::endl;
	}

	// Correct Divergence Errot
	template<typename T>
	void correct_divergence_error(T &kernel, int8_t boundaryMethod, size_t nbParticles, int8_t nbListSize, float mass, float rdensity, float coherenceError, float dt, float h,
		glm::vec3 *positions, glm::vec3 *velocity_i, int8_t *alive, int8_t *type, int8_t *nbCount, size_t *neighbours, float *boundary_mass, float *density_gradient, 
		float *alpha, glm::vec3 *pressureForce, float *ki01) {

		int iter = 0;
		float eta = (1.0f / dt) * coherenceError * 0.01f * rdensity;
		double rateAvg = 1e100;
		int pcount = 0;
		float _invdt = 1.f / dt;
		float h2 = h * h;

		calc_density_change(kernel, boundaryMethod, nbParticles, nbListSize, mass, positions, velocity_i, alive, type, nbCount, neighbours, boundary_mass, density_gradient);

		// Warm Start
		//#pragma omp parallel default(shared)
		//{
		//	#pragma omp for schedule(static)
		//	for (int i = 0; i < nbParticles; i++) {
		//		if (!alive[i]) continue;

		//		if (type[i] != FLUID_PARTICLE)
		//			continue;

		//		float ki_rho = ki01[i];

		//		glm::vec3 _pressureForce(0.0f);
		//		glm::vec3 boundaryPressureForce(0.0f);

		//		auto nbListOffset = i * nbListSize;
		//		for (int j = 0; j < nbCount[i]; j++) {

		//			if (!alive[neighbours[nbListOffset + j]]) continue;

		//			glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
		//			glm::vec3 grad = kernel.gradW(r);

		//			if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {

		//				float kj = density_gradient[neighbours[nbListOffset + j]] * alpha[neighbours[nbListOffset + j]]; //* _invdt;
		//				float kj_rho = kj;//np->density;	

		//				float ksum = ki_rho + kj_rho;

		//				if (ksum > RTX_MIN_E) {
		//					_pressureForce += mass * ksum * grad;
		//				}
		//			}
		//			else if (type[neighbours[nbListOffset + j]] != FLUID_PARTICLE) {

		//				if (ki_rho > RTX_MIN_E) {
		//					glm::vec3 _tempForce = boundary_mass[neighbours[nbListOffset + j]] * (ki_rho)* grad;
		//					_pressureForce -= _tempForce;
		//				}
		//			}
		//		}

		//		velocity_i[i] -= dt * _pressureForce;
		//		ki01[i] = 0.f;
		//	}
		//}

		///////////////////////////////////////////////////////////////////////////////
		while (((rateAvg > eta) || (iter < 2)) && (iter < 50)) {
			rateAvg = 0.0f;
			pcount = 0;

			//Adapt Velocities
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!alive[i]) continue;

					if (type[i] != FLUID_PARTICLE)
						continue;

					float ki = density_gradient[i] * alpha[i];// * _invdt;
					float ki_rho = ki;//particles[i]->density;

					ki01[i] += ki;

					glm::vec3 _pressureForce(0.0f);
					glm::vec3 boundaryPressureForce(0.0f);

					auto nbListOffset = i * nbListSize;
					for (int j = 0; j < nbCount[i]; j++) {

						if (!alive[neighbours[nbListOffset + j]]) continue;

						glm::vec3 r = positions[i] - positions[neighbours[nbListOffset + j]];
						glm::vec3 grad = kernel.gradW(r);

						if (type[neighbours[nbListOffset + j]] == FLUID_PARTICLE) {
							
							float kj = density_gradient[neighbours[nbListOffset + j]] * alpha[neighbours[nbListOffset + j]]; //* _invdt;
							float kj_rho = kj;//np->density;	

							float ksum = ki_rho + kj_rho;

							if (ksum > RTX_MIN_E) {
								_pressureForce += mass * ksum * grad;
							}
						}
						else if (boundaryMethod == AKINCI2012) {

							if (ki_rho > RTX_MIN_E) {
								glm::vec3 _tempForce = boundary_mass[neighbours[nbListOffset + j]] * (ki_rho) * grad;
								_pressureForce -= _tempForce;
								boundaryPressureForce -= _tempForce;
							}
						}
						else if (boundaryMethod == STATIC_PARTICLES) {

							if (ki_rho > RTX_MIN_E) {
								glm::vec3 _tempForce = mass * (ki_rho)* grad;
								_pressureForce -= _tempForce;
								boundaryPressureForce -= _tempForce;
							}
						}
					}

					pressureForce[i] += boundaryPressureForce;
					velocity_i[i] -= dt * _pressureForce;
				}
			}

			calc_density_change(kernel, boundaryMethod, nbParticles, nbListSize, mass, positions, velocity_i, alive, type, nbCount, neighbours, boundary_mass, density_gradient);

			for (int i = 0; i < nbParticles; i++) {
				if(!alive[i]) continue;

				if (type[i] != FLUID_PARTICLE)
					continue;

				rateAvg += density_gradient[i];

				++pcount;
			}

			rateAvg = rateAvg / pcount;
			iter++;
		}

		if (iter > 0) for (int i = 0; i < nbParticles; i++) ki01[i] *= h;
	}

	// Update CFL Condition
    inline float updateTimeStepSizeCFL(size_t nbParticles, glm::vec3 *velocity, glm::vec3 *forces, float current_timestep, float pradius, const float minTimeStepSize,
		const float maxTimeStepSize, float _cflFactor = 1.f) {
		const float radius = pradius;
		const float cflFactor = _cflFactor;
		float h = current_timestep;

		// Approximate max. position change due to current velocities
		float maxVel = 0.1f;
		float diameter = 2.0f * radius;
		for (unsigned int i = 0; i < nbParticles; i++)
		{
			const glm::vec3 vel   = velocity[i];
			const glm::vec3 accel = forces[i];
			const float velMag = pow2(glm::length((vel + accel * h)));
			if (velMag > maxVel)
				maxVel = velMag;
		}

		// Approximate max. time step size 		
		h = cflFactor * 0.4f * (diameter / (sqrt(maxVel)));

		h = std::min(h, maxTimeStepSize);
		h = std::max(h, minTimeStepSize);

		return h;
	}
}