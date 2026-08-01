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

#include "RtxSPHSystem.h"
#include "RtxCompactXSearch.h"

namespace rtx {

	SPHSystem::SPHSystem() {

		_nbParticles = 0;
		_nbListSize  = 50;

		_xsearch = nullptr;

		_positions = nullptr;
		_velocity = nullptr;
		_velocity_i = nullptr;
		_density   = nullptr;
		_density_i = nullptr;
		_forces   = nullptr;
		_type = nullptr;
		_alive = nullptr;
		_frozen = nullptr;

		_pressure_force = nullptr;
		_boundary_mass = nullptr;
		_alpha = nullptr;
		_density_gradient = nullptr;
		_ki00 = nullptr;
		_ki01 = nullptr;

		_stream_index = nullptr;

		_useViscosity = nullptr;
		_viscosity = nullptr;

		_useSurfaceTension = nullptr;
		_surfaceTension = nullptr;

		_useVorticity = nullptr;
		_vorticity = nullptr;

		_nbCount    = nullptr;
		_neighbours = nullptr;

	}

	SPHSystem::~SPHSystem() { }

	void SPHSystem::initBuffers(size_t size) {

		_positions = new glm::vec3[size];
		_velocity = new glm::vec3[size];
		_velocity_i = new glm::vec3[size];
		_forces = new glm::vec3[size];
		_density = new float[size];
		_density_i = new float[size];
		_type = new int8_t[size];
		_alive = new int8_t[size];
		_frozen= new int8_t[size];
		_pressure_force = new glm::vec3[size];
		_boundary_mass = new float[size];
		_alpha = new float[size];
		_density_gradient = new float[size];
		_ki00 = new float[size];
		_ki01 = new float[size];
		_stream_index = new size_t[size];
		_useViscosity = new int8_t[size];
		_viscosity = new float[size];
		_useSurfaceTension = new int8_t[size];
		_surfaceTension = new float[size];
		_useVorticity = new int8_t[size];
		_vorticity = new float[size];

		for (size_t inx = 0; inx < size; inx++) {
			_positions[inx] = glm::vec3(0.f);
			_velocity[inx] = glm::vec3(0.f);
			_velocity_i[inx] = glm::vec3(0.f);
			_forces[inx] = glm::vec3(0.f);
			_density[inx] = 0.f;
			_density_i[inx] = 0.f;
			_type[inx] = 0;
			_alive[inx] = 0;
			_frozen[inx] = 0;
			_pressure_force[inx] = glm::vec3(0.f);
			_boundary_mass[inx] = 0.f;
			_alpha[inx] = 0.f;
			_density_gradient[inx] = 0.f;
			_ki00[inx] = 0.f;
			_ki01[inx] = 0.f;
			_stream_index[inx] = 0;
			_useViscosity[inx] = 0;
			_viscosity[inx] = 0.f;
			_useSurfaceTension[inx] = 0;
			_surfaceTension[inx] = 0.f ;
			_useVorticity[inx] = 0;
			_vorticity[inx] = 0.f;
		}

		_nbParticles = size;
	}

	void SPHSystem::clearResize(size_t size) {
		
		clear();

		if (size > 0) {
			initBuffers(size);
		}
	}

	void SPHSystem::resize(size_t size) {

		if (size > 0) {

			auto tempPositions = _positions;
			auto tempVelocity = _velocity;
			auto tempVelocity_i = _velocity_i;
			auto tempForces = _forces;
			auto tempDensity = _density;
			auto tempDensity_i = _density_i;
			auto tempType = _type;
			auto tempAlive = _alive;
			auto tempFrozen = _frozen;
			auto tempPressure_force = _pressure_force;
			auto tempBoundary_mass = _boundary_mass;
			auto tempAlpha = _alpha;
			auto tempDensity_gradient = _density_gradient;
			auto tempKi00 = _ki00;
			auto tempKi01 = _ki01;
			auto tempStream_index = _stream_index;
			auto tempUseViscosity = _useViscosity;
			auto tempViscosity = _viscosity;
			auto tempUseSurfaceTension = _useSurfaceTension;
			auto tempSurfaceTension = _surfaceTension;
			auto tempUseVorticity = _useVorticity;
			auto tempVorticity = _vorticity;

			auto oldSize =_nbParticles;

			clearResize(size);
			
			if (oldSize <= 0) {
				clearResize(size);
			}
			else {
				initBuffers(size);
				_nbParticles = size;
	
				for (size_t inx = 0; inx < size; inx++) {
					if (inx == oldSize) break;

					_positions[inx]		= tempPositions[inx];
					_velocity[inx]		= tempVelocity[inx];
					_velocity_i[inx]	= tempVelocity_i[inx];
					_forces[inx]		= tempForces[inx];
					_density[inx]		= tempDensity[inx];
					_density_i[inx]		= tempDensity_i[inx];
					_type[inx]			= tempType[inx];
					_frozen[inx]		= tempFrozen[inx];
					_alive[inx]			= tempAlive[inx];
					_pressure_force[inx] = tempPressure_force[inx];
					_boundary_mass[inx]		= tempBoundary_mass[inx];
					_alpha[inx]				= tempAlpha[inx];
					_density_gradient[inx] = tempDensity_gradient[inx];
					_ki00[inx]			= tempKi00[inx];
					_ki01[inx]			= tempKi01[inx];
					_stream_index[inx]  = tempStream_index[inx];
					_useViscosity[inx]  = tempUseViscosity[inx];
					_viscosity[inx]		= tempViscosity[inx];
					_useSurfaceTension[inx] = tempUseSurfaceTension[inx];
					_surfaceTension[inx] = tempUseSurfaceTension[inx];
					_useVorticity[inx]	= tempUseVorticity[inx];
					_vorticity[inx]		= tempVorticity[inx];
				}

				delete[] tempPositions;
				delete[] tempVelocity;
				delete[] tempVelocity_i;
				delete[] tempForces;
				delete[] tempDensity;
				delete[] tempDensity_i;
				delete[] tempType;
				delete[] tempAlive;
				delete[] tempFrozen;
				delete[] tempPressure_force;
				delete[] tempBoundary_mass;
				delete[] tempAlpha;
				delete[] tempDensity_gradient;
				delete[] tempKi00;
				delete[] tempKi01;
				delete[] tempStream_index;
				delete[] tempUseViscosity;
				delete[] tempViscosity;
				delete[] tempUseSurfaceTension;
				delete[] tempSurfaceTension;
				delete[] tempUseVorticity;
				delete[] tempVorticity;
			}
			
		}
	}

	void SPHSystem::clear() {

		if (_positions != nullptr) delete[] _positions;
		if (_velocity != nullptr) delete[] _velocity;
		if (_velocity_i != nullptr) delete[] _velocity_i;
		if (_forces   != nullptr) delete[] _forces;
		if (_type != nullptr) delete[] _type;
		if (_alive != nullptr) delete[] _alive;
		if (_frozen != nullptr) delete[] _frozen;
		if (_boundary_mass != nullptr) delete[] _boundary_mass;
		if (_density != nullptr) delete[] _density;
		if (_density_i != nullptr) delete[] _density_i;
		if (_alpha != nullptr) delete[] _alpha;
		if (_density_gradient != nullptr) delete[] _density_gradient;
		if (_pressure_force != nullptr) delete[] _pressure_force;
		if (_ki00 != nullptr) delete[] _ki00;
		if (_ki01 != nullptr) delete[] _ki01;
		if (_stream_index != nullptr) delete[] _stream_index;
		if (_viscosity != nullptr) delete[] _viscosity;
		if (_useSurfaceTension != nullptr) delete[] _useSurfaceTension;
		if (_surfaceTension != nullptr) delete[] _surfaceTension;
		if (_useVorticity != nullptr) delete[] _useVorticity;
		if (_vorticity != nullptr) delete[] _vorticity;
		
		_positions = nullptr;
		_velocity = nullptr;
		_velocity_i = nullptr;
		_forces   = nullptr;
		_density = nullptr;
		_density_i = nullptr;
		_type = nullptr;
		_alive = nullptr;
		_frozen = nullptr;
		_boundary_mass = nullptr;
		_alpha = nullptr;
		_density_gradient = nullptr;
		_pressure_force = nullptr;
		_ki00 = nullptr;
		_ki01 = nullptr;
		_stream_index = nullptr;
		_useViscosity = nullptr;
		_viscosity = nullptr;
		_useSurfaceTension = nullptr;
		_surfaceTension = nullptr;
		_useVorticity = nullptr;
		_vorticity = nullptr;

		_nbParticles = 0;
	}

	void SPHSystem::clearOther() {
		_heaters.clear();
	}

	void SPHSystem::clearNeighbourList() {

		for (size_t inx = 0; inx < _nbParticles; inx++) _nbCount[inx] = 0;
		for (size_t inx = 0; inx < (_nbParticles * _nbListSize); inx++) _neighbours[inx] = 0;	

	}

	void SPHSystem::initNeighbourList() {

		if (_nbCount != nullptr) delete[] _nbCount;
		if (_neighbours != nullptr) delete[] _neighbours;

		_nbCount = nullptr;
		_neighbours = nullptr;

		if (_nbParticles > 0 && _nbListSize > 0) {
			_nbCount = new int8_t[_nbParticles];
			_neighbours = new size_t[_nbParticles * _nbListSize];
		}

	}

	void SPHSystem::deleteNeighbourList() {

		if (_nbCount != nullptr) delete[] _nbCount;
		if (_neighbours != nullptr) delete[] _neighbours;

		_nbCount = nullptr;
		_neighbours = nullptr;

	}

	glm::vec3 *SPHSystem::positions() {
		return _positions;
	}

	glm::vec3 *SPHSystem::velocity() {
		return _velocity;
	}

	glm::vec3 *SPHSystem::velocity_i() {
		return _velocity_i;
	}

	glm::vec3 *SPHSystem::forces() {
		return _forces;
	}

	float *SPHSystem::density() {
		return _density;
	}

	float *SPHSystem::density_i() {
		return _density_i;
	}

	float *SPHSystem::density_gradient() {
		return _density_gradient;
	}

	float *SPHSystem::ki00() {
		return _ki00;
	}

	float *SPHSystem::ki01() {
		return _ki01;
	}

	glm::vec3 *SPHSystem::pressure_force() {
		return _pressure_force;
	}

	int8_t *SPHSystem::type() {
		return _type;
	}

	int8_t *SPHSystem::frozen() {
		return _frozen;
	}

	int8_t *SPHSystem::alive() {
		return _alive;
	}

	float *SPHSystem::boundary_mass() {
		return _boundary_mass;
	}

	float *SPHSystem::alpha() {
		return _alpha;
	}

	size_t *SPHSystem::stream_index() {
		return _stream_index;
	}

	int8_t *SPHSystem::useViscosity() {
		return _useViscosity;
	}

	float  *SPHSystem::viscosity() {
		return _viscosity;
	}

	int8_t *SPHSystem::useSurfaceTension() {
		return _useSurfaceTension;
	}

	float  *SPHSystem::surfaceTension() {
		return _surfaceTension;
	}

	int8_t *SPHSystem::useVorticity() {
		return _useVorticity;
	}

	float  *SPHSystem::vorticity() {
		return _vorticity;
	}

	size_t SPHSystem::nbParticles() {
		return _nbParticles;
	}

	int8_t SPHSystem::nbListSize() {
		return _nbListSize;
	}

	size_t *SPHSystem::neighbours() {
		return _neighbours;
	}

	int8_t *SPHSystem::nbCount() {
		return _nbCount;
	}

	// 
	float SPHSystem::rdensity() {
		return _rdensity;
	}

	float SPHSystem::pradius() {
		return _pradius;
	}

	float SPHSystem::pspacing() {
		return _pspacing;
	}

	float SPHSystem::h() {
		return _h;
	}

	float SPHSystem::diam() {
		return _diam;
	}

	float SPHSystem::mass() {
		return _mass;
	}

	void SPHSystem::setParticleRadius(float pr) {
		_pradius = pr;
		_pspacing = _pradius * 2.f;
		_h = _pradius * 4.f;
		_diam = _pradius * 2.f;
		_mass = _diam * _diam * _diam * _rdensity;

		_kernel.setRadius(_h);
	}

	void SPHSystem::setRestDensity(float ds) {
		_rdensity = ds;
	}

	rtx::PrecomputedKernel<rtx::CubicKernel> &SPHSystem::kernel() {
		return _kernel;
	}

	void SPHSystem::resetNeighbourSearch() {
		if (_xsearch == nullptr) delete _xsearch;

		initNeighbourList();

		_xsearch = new CompactXSearch;
		_xsearch->reset(this);
		_xsearch->setRadius(_h);
	}

	void SPHSystem::findNeighbours() {
		_xsearch->findNeighbours();
	}

	int	SPHSystem::newHeater() {
		int index = _heaters.size();
		
		Heater heater;
		heater.setPosition(glm::vec3(0.f));
		heater.setRadius(1.f);
		heater.setIndex(index);
		_heaters.push_back(heater);

		return index;
	}

	std::vector<Heater> &SPHSystem::heaters() {
		return _heaters;
	}
}