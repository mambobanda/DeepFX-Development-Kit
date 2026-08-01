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
#include "DfxFoamCompactSearch.h"


namespace dfxcore {

		FoamCompactSearch::FoamCompactSearch() {
			neighbourBufferSize = 80;
		}

		void FoamCompactSearch::findNeighboursInCell(sph::Particle **particles, FoamParticle *fparticles, size_t index, float _h) {
			int inx = 0;
			while (particles[inx] != nullptr) {

				float vlength = glm::distance(fparticles[index].pos, particles[inx]->x);

				if (vlength < (_h + 1.0e-5f)) {
					fparticles[index].nbs.push_back(particles[inx]->getIndex());
				}
			
				++inx;
			}
		}

		glm::vec3 *FoamCompactSearch::getNeighbourCells(glm::vec3 position, float xm, float ym, float zm) {
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

		void FoamCompactSearch::findNeighbours(FoamParticle *fparticles, size_t foamcount, sph::Particle **particles, int nbParticles, double _h, double _pradius) {


			std::unordered_map<glm::vec3, sph::Particle**, SpatialHash> compact_hash(nbParticles * 2);

			neighbour_search(fparticles, foamcount, particles, nbParticles, compact_hash, _h, _pradius);
		}

		template<typename T>
		void FoamCompactSearch::neighbour_search(FoamParticle *fparticles, size_t foamcount, sph::Particle **particles, 
			int particleCount, T compact_hash, float dfsph_h, float dfsph_pradius) {

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


			/*if( xcellCount == 0 || xcellCount == 0 || xcellCount == 0){

			stringstream ss;
			ss << "XCount: " << xcellCount << endl;
			ss << "YCount: " << ycellCount << endl;
			ss << "ZCount: " << zcellCount << endl;
			ss << "XLength: " <<  xlength << endl;
			ss << "YLength: " <<  ylength << endl;
			ss << "ZLength: " <<  zlength << endl;
			ss << "Xmin: " << bbox.xmax  << endl;
			ss << "Ymin: " << bbox.ymax  << endl;
			ss << "Zmin: " << bbox.zmax  << endl;

			appendToFile("c:/test/compact.txt", (char*)ss.str().c_str());
			}*/

			for (int i = 0; i < particleCount; i++) {
				sph::Particle *p = particles[i];

				glm::vec3 location = getSpatialLocation(p->x, bbox.xmin, bbox.ymin, bbox.zmin);
				T::iterator it = compact_hash.find(location);

				if (it != compact_hash.end()) {

					int counter = 0;
					while (true) {
						if (counter < neighbourBufferSize) {
							if (it->second[counter] == nullptr) {
								it->second[counter] = p;
								it->second[++counter] = nullptr;
								break;
							}
						}
						else
							break;

						++counter;
					}
				}
				else {
					sph::Particle **cell = new sph::Particle*[neighbourBufferSize + 1];
					cell[0] = p;
					cell[1] = nullptr;
					compact_hash[location] = cell;
				}
			}

			int list_size = compact_hash.size();
			sph::Particle ***cell_list = new sph::Particle**[list_size];

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
				for (int i = 0; i < foamcount; i++) {

					fparticles[i].nbs.clear();

					// Find Neighrtbour Particles/////////////////////////////////////////////////////////////////
					glm::vec3 *nb_locations = getNeighbourCells(fparticles[i].pos, bbox.xmin, bbox.ymin, bbox.zmin);

					for (int inx = 0; inx < 27; inx++) {

						T::iterator it = compact_hash.find(nb_locations[inx]);

						if (it != compact_hash.end()) {
							findNeighboursInCell(it->second, fparticles, i, dfsph_h);
						}

					}

					delete[] nb_locations;

				}
			}

			for (int inx = 0; inx < list_counter; inx++) {
				delete[] cell_list[inx];
			}

			delete[] cell_list;
		}


		int FoamCompactSearch::getNeighbourBufferSize() {
			return neighbourBufferSize;
		}

		void FoamCompactSearch::setNeighbourBufferSize(int nbs) {
			neighbourBufferSize = nbs;
		}
}