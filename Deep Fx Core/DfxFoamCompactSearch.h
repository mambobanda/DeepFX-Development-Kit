/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include <glm\vec3.hpp>
#include <DfxBounds.h>
#include "DfxCSpatialHash.h"
#include "DfxCSPHParticle.h"
#include "DfxFoamParticle.h"


#define MAP_SPARSE 0
#define MAP_DENSE 1
#define MAP_BALANCED 2


namespace dfxcore {

		class FoamCompactSearch {
		private:
			int neighbourBufferSize;

		public:
			FoamCompactSearch();
			void findNeighbours(FoamParticle *fparticles, size_t foamcount, sph::Particle **particles, int nbParticles, double _h, double _pradius);

			template<typename T>
			void neighbour_search(FoamParticle *fparticles, size_t foamcount, sph::Particle **particles, int particleCount, T compact_hash, float dfsph_h, float dfsph_pradius);

			int getNeighbourBufferSize();
			void setNeighbourBufferSize(int);

			glm::vec3 *getNeighbourCells(glm::vec3 position, float, float, float);

			void findNeighboursInCell(sph::Particle **particles, FoamParticle *fparticles, size_t index, float h);
		};
}
