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

#include "RtxCompactSearch.h"
#include <sstream>

namespace rtx {

	CompactSearch::CompactSearch() {
		neighbourBufferSize = 80;
	}

	void CompactSearch::findNeighboursInCell(std::vector<size_t> &particles, size_t p, int &counter, int8_t nbListSize, float h, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours) {

		bool isBoundaryParticle = false;

		if (!alive[p])return;

		if (type[p] == BOUNDARY_PARTICLE) isBoundaryParticle = true;

		if (type[p] == MOVING_BOUNDARY_PARTICLE) isBoundaryParticle = true;

		int inx = 0;
		for (; inx < particles.size(); inx++) {

			if (!alive[particles[inx]]) continue;

			float vlength = glm::distance(positions[p], positions[particles[inx]]);

			if (vlength < (h + 1.0e-5f)) {

				/*if (isBoundaryParticle) {

					if (particles[inx]->getType() == FLUID_PARTICLE) {
						if (nbcount < neighbourBufferSize) {
							p->nbnbIndices[nbcount] = particles[inx]->getIndex();
							p->nbnbCount = ++nbcount;
							continue;
						}
					}

				}*/

				auto nbListOffset = p * nbListSize;
				if (counter < nbListSize && p != particles[inx]) {
					neighbours[nbListOffset + counter] = particles[inx];
					nbCount[p] = ++counter;
				}
			}
		}
	}

	glm::vec3 *CompactSearch::getNeighbourCells(glm::vec3 position, float xm, float ym, float zm) {
		glm::vec3 *nbs = new glm::vec3[27];

		glm::vec3 l = getSpatialLocation(position, xm, ym, zm);

		nbs[0] = glm::vec3(l.x - 1.f, l.y - 1.f, l.z - 1.f);  nbs[1] = glm::vec3(l.x, l.y - 1.f, l.z - 1.f); nbs[2] = glm::vec3(l.x + 1.f, l.y - 1.f, l.z - 1.f);
		nbs[3] = glm::vec3(l.x - 1.f, l.y, l.z - 1.f);      nbs[4] = glm::vec3(l.x, l.y, l.z - 1.f);     nbs[5] = glm::vec3(l.x + 1.f, l.y, l.z - 1.f);
		nbs[6] = glm::vec3(l.x - 1.f, l.y + 1.f, l.z - 1.f);  nbs[7] = glm::vec3(l.x, l.y + 1.f, l.z - 1.f); nbs[8] = glm::vec3(l.x + 1.f, l.y + 1.f, l.z - 1.f);

		nbs[9] = glm::vec3(l.x - 1.f, l.y - 1.f, l.z);  nbs[10] = glm::vec3(l.x, l.y - 1.f, l.z); nbs[11] = glm::vec3(l.x + 1.f, l.y - 1.f, l.z);
		nbs[12] = glm::vec3(l.x - 1.f, l.y, l.z);      nbs[13] = glm::vec3(l.x, l.y, l.z);     nbs[14] = glm::vec3(l.x + 1.f, l.y, l.z);
		nbs[15] = glm::vec3(l.x - 1.f, l.y + 1.f, l.z);  nbs[16] = glm::vec3(l.x, l.y + 1.f, l.z); nbs[17] = glm::vec3(l.x + 1.f, l.y + 1.f, l.z);

		nbs[18] = glm::vec3(l.x - 1.f, l.y - 1.f, l.z + 1.f); nbs[19] = glm::vec3(l.x, l.y - 1.f, l.z + 1.f); nbs[20] = glm::vec3(l.x + 1.f, l.y - 1.f, l.z + 1.f);
		nbs[21] = glm::vec3(l.x - 1.f, l.y, l.z + 1.f);     nbs[22] = glm::vec3(l.x, l.y, l.z + 1.f);     nbs[23] = glm::vec3(l.x + 1.f, l.y, l.z + 1.f);
		nbs[24] = glm::vec3(l.x - 1.f, l.y + 1.f, l.z + 1.f); nbs[25] = glm::vec3(l.x, l.y + 1.f, l.z + 1.f); nbs[26] = glm::vec3(l.x + 1.f, l.y + 1.f, l.z + 1.f);

		return nbs;
	}

	void CompactSearch::findNeighbours(size_t nbParticles, int8_t nbListSize, float h, float radius, glm::vec3 *positions, int8_t *alive, int8_t *type,
		int8_t *nbCount, size_t *neighbours) {

		std::unordered_map<glm::vec3, std::vector<size_t>, SpatialHash> compact_hash(nbParticles * 2);

		neighbour_search(nbParticles, nbListSize, h, radius, compact_hash, positions, alive, type, nbCount, neighbours);
	}

	int CompactSearch::getNeighbourBufferSize() {
		return neighbourBufferSize;
	}

	void CompactSearch::setNeighbourBufferSize(int nbs) {
		neighbourBufferSize = nbs;
	}

}