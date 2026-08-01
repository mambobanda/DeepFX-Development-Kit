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

#include <glm/glm.hpp>
#include <vector>
#include "RtxCommon.h"
#include "RtxRiptideKernels.h"
#include "RtxHeater.h"

namespace rtx {

	class SPHSystem {
	private:
		rtx::CompactXSearch *_xsearch;
		rtx::PrecomputedKernel<rtx::CubicKernel> _kernel;
		std::vector<Heater> _heaters;
	private:
		float _rdensity;
		float _pradius;
		float _pspacing;
		float _h;
		float _diam;
		float _mass;

	private:
		void initBuffers(size_t size);
	private:
		size_t _nbParticles;
		int8_t _nbListSize;

		size_t *_neighbours;
		int8_t *_nbCount;

		glm::vec3 *_positions;
		glm::vec3 *_velocity;
		glm::vec3 *_velocity_i;
		glm::vec3 *_forces;
		float  *_density;
		float  *_density_i;
		int8_t *_alive;
		int8_t *_type;
		int8_t *_frozen;

		glm::vec3 *_pressure_force;
		float *_boundary_mass;
		float *_alpha;
		float *_density_gradient;
		float *_ki00;
		float *_ki01;

		size_t *_stream_index;

		int8_t *_useViscosity;
		float  *_viscosity;

		int8_t *_useSurfaceTension;
		float  *_surfaceTension;

		int8_t *_useVorticity;
		float  *_vorticity;

		size_t _size;
	public:

		SPHSystem();
		virtual ~SPHSystem();

		void clearResize(size_t size);
		void resize(size_t size);
		void clear();
		void clearOther();
		void initNeighbourList();
		void clearNeighbourList();
		void deleteNeighbourList();

		size_t *neighbours();
		int8_t *nbCount();

		glm::vec3 *positions();
		glm::vec3 *velocity();
		glm::vec3 *velocity_i();
		glm::vec3 *forces();
		float *density();
		float *density_i();
		int8_t *alive();
		int8_t *type();
		int8_t *frozen();
		
		float *boundary_mass();
		float *alpha();
		float *density_gradient();
		glm::vec3 *pressure_force();
		float *ki00();
		float *ki01();

		size_t *stream_index();

		int8_t *useViscosity();
		float  *viscosity();

		int8_t *useSurfaceTension();
		float  *surfaceTension();

		int8_t *useVorticity();
		float  *vorticity();

		size_t nbParticles();
		int8_t nbListSize();

	public:

		float rdensity();
		float pradius();
		float pspacing();
		float h();
		float diam();
		float mass();
		rtx::PrecomputedKernel<rtx::CubicKernel> &kernel();

		void resetNeighbourSearch();
		void findNeighbours();

		void setParticleRadius(float pr);
		void setRestDensity(float ds);

		int	newHeater();
		std::vector<Heater> &heaters();
	};
}