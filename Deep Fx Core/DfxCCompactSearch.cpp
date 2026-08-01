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

#include "DfxCCompactSearch.h"
#include "DfxGlobal.h"
#include <sstream>

namespace dfxcore {

	namespace sph {
		CompactSearch::CompactSearch() {
			neighbourBufferSize = 60;
		}

		void CompactSearch::findNeighboursInCell(std::vector<Particle*> &particles, Particle* p, int &counter, int &nbcount, float _h) {
			bool isBoundaryParticle = false;

			if (!p->isAlive())return;

			if (p->getType() == BOUNDARY_PARTICLE) isBoundaryParticle = true;

			if (p->getType() == MOVING_BOUNDARY_PARTICLE) isBoundaryParticle = true;

			int inx = 0;
			for (; inx < particles.size(); inx++) {

				if (!particles[inx]->isAlive()) continue;
				

				float vlength = glm::distance(p->x, particles[inx]->x);

				if (p->getIndex() == 0)
					int i = 0;

				if (vlength < (_h + 1.0e-5f)) {

					/*if (isBoundaryParticle) {

						if (particles[inx]->getType() == FLUID_PARTICLE) {
							if (nbcount < neighbourBufferSize) {
								p->nbnbIndices[nbcount] = particles[inx]->getIndex();
								p->nbnbCount = ++nbcount;
								continue;
							}
						}

					}*/

					if (counter < neighbourBufferSize) {
						p->nbIndices[counter] = particles[inx]->getIndex();
						p->nbCount = ++counter;						
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

		void CompactSearch::findNeighbours(Particle **particles, int nbParticles, double _h, double _pradius) {

			std::unordered_map<glm::vec3, std::vector<Particle*>, SpatialHash> compact_hash(nbParticles * 2);

			neighbour_search(particles, nbParticles, compact_hash, _h, _pradius);
		}

		template<typename T>
		void CompactSearch::neighbour_search(Particle **particles, int particleCount, T compact_hash, float dfsph_h, float dfsph_pradius) {

			DfxBounds bbox;

			bbox.empty();

			for (int i = 0; i < particleCount; i++)
			bbox.add(particles[i]->x.x, particles[i]->x.y, particles[i]->x.z);

			bbox.addPadding(dfsph_pradius, dfsph_pradius, dfsph_pradius);

			float xlength = std::abs((bbox.xmax - bbox.xmin)) + dfsph_h;
			float ylength = std::abs((bbox.ymax - bbox.ymin)) + dfsph_h;
			float zlength = std::abs((bbox.zmax - bbox.zmin)) + dfsph_h;

			int xcellCount = (int)std::max(1.0f, (xlength / dfsph_h));
			int ycellCount = (int)std::max(1.0f, (ylength / dfsph_h));
			int zcellCount = (int)std::max(1.0f, (zlength / dfsph_h));

			int totalCellCount = xcellCount * ycellCount * zcellCount;

			setSpatialHashAttributes(dfsph_h, totalCellCount);

			/*{
				std::stringstream ss;
				ss << "XCount: " << xcellCount << std::endl;
				ss << "YCount: " << ycellCount << std::endl;
				ss << "ZCount: " << zcellCount << std::endl;
				ss << "XLength: " << xlength << std::endl;
				ss << "YLength: " << ylength << std::endl;
				ss << "ZLength: " << zlength << std::endl;
				ss << "Xmin: " << bbox.xmax << std::endl;
				ss << "Ymin: " << bbox.ymax << std::endl;
				ss << "Zmin: " << bbox.zmax << std::endl;

				DfxGlobal::console::println(ss.str());
			}*/
		

			for (int i = 0; i < particleCount; i++) {
				Particle *p = particles[i];

				glm::vec3 location = getSpatialLocation(p->x, bbox.xmin, bbox.ymin, bbox.zmin);
				T::iterator it = compact_hash.find(location);

				if (it != compact_hash.end()) {

					if (p->isAlive()) {
						it->second.push_back(p);
					}
				}
				else {

					if (p->isAlive()) {
						std::vector<Particle*> cell;
						cell.push_back(p);
						compact_hash[location] = cell;
					}
				}
			}

			int list_size = compact_hash.size();
			std::vector<Particle*> *cell_list = new std::vector<Particle*>[list_size];

			int list_counter = 0;
			T::iterator list_it = compact_hash.begin();
			list_it = compact_hash.begin();
			for (; list_it != compact_hash.end(); list_it++) {
				cell_list[list_counter] = list_it->second;
				list_counter++;
			}

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < list_size; i++) {

					std::vector<Particle*> c = cell_list[i];

					int jnx = 0;
					for (; jnx < c.size(); jnx++) {
		
						// Local Variables
						Particle *p = c[jnx];
						int counter = 0;
						int nbcounter = 0;

						// Clear old neighbours
						if (p->nbIndices != NULL) {
							delete p->nbIndices;
							p->nbCount = 0;
						}

					/*	if (p->nbnbIndices != NULL) {
							delete p->nbnbIndices;
							p->nbnbCount = 0;
						}*/

						// Create space for new neighbours 
						p->nbIndices = new unsigned int[neighbourBufferSize];
						// p->nbnbIndices = new unsigned int[neighbourBufferSize];

						// p->nbnbIndices = new unsigned int[neighbourBufferSize];

						// Find Neighrtbour Particles /////////////////////////////////////////////////////////////////
						glm::vec3 *nb_locations = getNeighbourCells(p->x, bbox.xmin, bbox.ymin, bbox.zmin);

						for (int inx = 0; inx < 27; inx++) {

							T::iterator it = compact_hash.find(nb_locations[inx]);

							if (it != compact_hash.end()) {
								findNeighboursInCell(it->second, p, counter, nbcounter, dfsph_h);
							}

						}

						delete[] nb_locations;

						//++jnx;
					}
				}

			}
			/*for (int inx = 0; inx < list_counter; inx++) {
				delete[] cell_list[inx];
			}*/

			delete[] cell_list;
		}


		int CompactSearch::getNeighbourBufferSize() {
			return neighbourBufferSize;
		}

		void CompactSearch::setNeighbourBufferSize(int nbs) {
			neighbourBufferSize = nbs;
		}
	}
}