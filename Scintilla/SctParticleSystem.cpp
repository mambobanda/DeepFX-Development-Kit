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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "SctParticleSystem.h"

namespace sct {
	
	ParticleSystem::ParticleSystem() {

		_nbParticles = 0;
		_nbListSize = 50;

		_xsearch = nullptr;

		_positions = nullptr;
		_positions_i = nullptr;
		_velocity = nullptr;
		_forces = nullptr;
		_type = nullptr;
		_alive = nullptr;
		_frozen = nullptr;

		_nbCount = nullptr;
		_neighbours = nullptr;

	}

	ParticleSystem::~ParticleSystem() {

	}

	void ParticleSystem::initBuffers(size_t size) {

		_positions = new glm::vec3[size];
		_positions_i = new glm::vec3[size];
		_velocity = new glm::vec3[size];
		_forces = new glm::vec3[size];
		_type = new int8_t[size];
		_alive = new int8_t[size];
		_frozen = new int8_t[size];

		for (size_t inx = 0; inx < size; inx++) {
			_positions[inx] = glm::vec3(0.f);
			_positions_i[inx] = glm::vec3(0.f);
			_velocity[inx] = glm::vec3(0.f);
			_forces[inx] = glm::vec3(0.f);
			_type[inx] = 0;
			_alive[inx] = 0;
			_frozen[inx] = 0;
		}

		_nbParticles = size;
	}

	void ParticleSystem::clearResize(size_t size) {
		clear();

		if (size > 0) {
			initBuffers(size);
		}
	}

	void ParticleSystem::resize(size_t size) {
		if (size > 0) {

			auto tempPositions = _positions;
			auto tempPositions_i = _positions_i;
			auto tempVelocity = _velocity;
			auto tempForces = _forces;
			auto tempType = _type;
			auto tempAlive = _alive;
			auto tempFrozen = _frozen;

			auto oldSize = _nbParticles;

			clearResize(size);

			if (oldSize <= 0) {
				clearResize(size);
			}
			else {
				initBuffers(size);
				_nbParticles = size;

				for (size_t inx = 0; inx < size; inx++) {
					if (inx == oldSize) break;

					_positions[inx] = tempPositions[inx];
					_positions_i[inx] = tempPositions_i[inx];
					_velocity[inx] = tempVelocity[inx];
					_forces[inx] = tempForces[inx];
					_type[inx] = tempType[inx];
					_frozen[inx] = tempFrozen[inx];
					_alive[inx] = tempAlive[inx];
				}

				delete[] tempPositions;
				delete[] tempPositions_i;
				delete[] tempVelocity;
				delete[] tempForces;
				delete[] tempType;
				delete[] tempAlive;
				delete[] tempFrozen;
			}

		}
	}

	void ParticleSystem::clear() {

		if (_positions != nullptr) delete[] _positions;
		if (_positions_i != nullptr) delete[] _positions_i;
		if (_velocity != nullptr) delete[] _velocity;
		if (_forces != nullptr) delete[] _forces;
		if (_type != nullptr) delete[] _type;
		if (_alive != nullptr) delete[] _alive;
		if (_frozen != nullptr) delete[] _frozen;

		_positions = nullptr;
		_positions_i = nullptr;
		_velocity = nullptr;
		_forces = nullptr;
		_type = nullptr;
		_alive = nullptr;
		_frozen = nullptr;

		_nbParticles = 0;
	}

	void ParticleSystem::clearOther() {

	}

	void ParticleSystem::initNeighbourList() {

		if (_nbCount != nullptr) delete[] _nbCount;
		if (_neighbours != nullptr) delete[] _neighbours;

		_nbCount = nullptr;
		_neighbours = nullptr;

		if (_nbParticles > 0 && _nbListSize > 0) {
			_nbCount = new int8_t[_nbParticles];
			_neighbours = new size_t[_nbParticles * _nbListSize];
		}

	}

	void ParticleSystem::clearNeighbourList() {
		for (size_t inx = 0; inx < _nbParticles; inx++) _nbCount[inx] = 0;
		for (size_t inx = 0; inx < (_nbParticles * _nbListSize); inx++) _neighbours[inx] = 0;
	}

	void ParticleSystem::deleteNeighbourList() {

		if (_nbCount != nullptr) delete[] _nbCount;
		if (_neighbours != nullptr) delete[] _neighbours;

		_nbCount = nullptr;
		_neighbours = nullptr;

	}

	size_t *ParticleSystem::neighbours() {
		return _neighbours;
	}

	int8_t *ParticleSystem::nbCount() {
		return _nbCount;
	}

	glm::vec3 *ParticleSystem::positions() {
		return _positions;
	}

	glm::vec3 *ParticleSystem::positions_i() {
		return _positions_i;
	}

	glm::vec3 *ParticleSystem::velocity() {
		return _velocity;
	}

	glm::vec3 *ParticleSystem::forces() {
		return _forces;
	}

	int8_t *ParticleSystem::alive() {
		return _alive;
	}

	int8_t *ParticleSystem::type() {
		return _type;
	}

	int8_t *ParticleSystem::frozen() {
		return _frozen;
	}

	size_t ParticleSystem::nbParticles() {
		return _nbParticles;
	}

	int8_t ParticleSystem::nbListSize() {
		return _nbListSize;
	}
}