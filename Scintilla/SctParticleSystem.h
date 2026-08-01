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
#include <RtxCompactXSearch.h>

namespace sct {
	class ParticleSystem {
	private:
		rtx::CompactXSearch *_xsearch;

	private:
		size_t _nbParticles;
		int8_t _nbListSize;

		size_t *_neighbours;
		int8_t *_nbCount;

		glm::vec3 *_positions;
		glm::vec3 *_positions_i;
		glm::vec3 *_velocity;
		glm::vec3 *_forces;
		int8_t *_alive;
		int8_t *_type;
		int8_t *_frozen;

		size_t _size;
	private:
		void initBuffers(size_t size);

	public:

		ParticleSystem();
		virtual ~ParticleSystem();

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
		glm::vec3 *positions_i();
		glm::vec3 *velocity();
		glm::vec3 *forces();
		int8_t *alive();
		int8_t *type();
		int8_t *frozen();

		size_t nbParticles();
		int8_t nbListSize();
	};
}