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

#include "DfxCPBDGrainCompactSearch.h"

namespace dfxcore {

	namespace pbd {

		glm::vec3 CompactSearch::getSpatialLocation(glm::vec3 p) {

			glm::vec3 res;
			res.x = floor((p.x - min_x) / h);
			res.y = floor((p.y - min_y) / h);
			res.z = floor((p.z - min_z) / h);

			return res;
		}

		size_t CompactSearch::getSpatialHash(glm::vec3 p) {

			size_t _x = (size_t)p.x * 73856093;
			size_t _y = (size_t)p.y * 19349663;
			size_t _z = (size_t)p.z * 83492791;

			return (_x ^ _y ^ _z) % cell_count;
		}

		int  CompactSearch::findMaxGridCellValue(Particle **particles, int particleCount) {
			size_t max = 0;

			for (int i = 0; i < particleCount; i++)
				if (references[i].gridIndex > max)
					max = references[i].gridIndex;

			return max + 1;
		}

		CompactSearch::CompactSearch(int neighbourListSize) {

			this->neighbourListSize = neighbourListSize;

			//if(particleCount > 0){
			//	this->particles = particles;
			//	this->particleCount = particleCount;
			//	this->h = _h;

			//	xtimer::Timer xt;
			//	xt.startTimer();
			//	
			//	minMax();

			//	xt.stopTimer();
			//	cout << "\n Neighbour Search MinMAX time: " << xt.getTime() << "ms\n\n";

			//	float xlength = std::abs((max_x - min_x));// + h;
			//	float ylength = std::abs((max_y - min_x));// + h;
			//	float zlength = std::abs((max_z - min_x));// + h;
			//
			//	int xcellCount = (int)(xlength/(h));
			//	int ycellCount = (int)(ylength/(h));
			//	int zcellCount = (int)(zlength/(h));

			//	cell_count = xcellCount*ycellCount*zcellCount;

			//	if(cell_count == 0) cell_count = 1;

			//	references = new GridReference[particleCount];
			//}
		}

		void CompactSearch::minMax(Particle **particles, int particleCount) {

			float xmin, xmax;
			float ymin, ymax;
			float zmin, zmax;

			xmin = ymin = zmin = std::numeric_limits<float>::infinity();
			xmax = ymax = zmax = -std::numeric_limits<float>::infinity();

			for (int i = 0; i < particleCount; i++) {

				float x, y, z;

				x = particles[i]->x.x;
				y = particles[i]->x.y;
				z = particles[i]->x.z;

				if (x < xmin) xmin = x;
				if (x > xmax) xmax = x;
				if (y < ymin) ymin = y;
				if (y > ymax) ymax = y;
				if (z < zmin) zmin = z;
				if (z > zmax) zmax = z;
			}

			max_x = xmax;
			max_y = ymax;
			max_z = zmax;

			min_x = xmin;
			min_y = ymin;
			min_z = zmin;
		}

		void CompactSearch::search() {

		}

		void CompactSearch::hashParticles(Particle **particles, int particleCount) {

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < particleCount; i++) {

					glm::vec3 p = getSpatialLocation(particles[i]->x);

					references[i].particleID = i;
					references[i].gridIndex = getSpatialHash(p);
				}
			}

		}

		void CompactSearch::sortAndIndexReferences(Particle **particles, int particleCount) {
			if (particleCount > 0) {
				std::sort(references, references + particleCount, referenceSort);

				cellHashMax = findMaxGridCellValue(particles, particleCount);
				gridMap = new int[cellHashMax]();
				memset(gridMap, -1, sizeof(int)*cellHashMax);

				/*int inx = references[0].gridIndex;
				references[0].head = inx;*/

				int inx = -1;
				/*int currentCell = -1;*/

				for (int i = 1; i < particleCount; i++) {

					if (inx != references[i].gridIndex) {
						inx = references[i].gridIndex;
						gridMap[inx] = i;
					}
				}
			}
		}

		bool CompactSearch::referenceSort(GridReference a, GridReference b) {
			return a.gridIndex < b.gridIndex;
		}


		size_t CompactSearch::_c_(glm::vec3 position) {
			return getSpatialHash(position);
		}

		size_t *CompactSearch::getNeighbourCells(glm::vec3 position) {
			size_t *nbs = new size_t[27];

			glm::vec3 l = getSpatialLocation(position);

			nbs[0] = _c_(glm::vec3(l.x - 1.f, l.y - 1.f, l.z - 1.f)); nbs[1] = _c_(glm::vec3(l.x, l.y - 1.f, l.z - 1.f)); nbs[2] = _c_(glm::vec3(l.x + 1.f, l.y - 1.f, l.z - 1.f));
			nbs[3] = _c_(glm::vec3(l.x - 1.f, l.y, l.z - 1.f));     nbs[4] = _c_(glm::vec3(l.x, l.y, l.z - 1.f));     nbs[5] = _c_(glm::vec3(l.x + 1.f, l.y, l.z - 1.f));
			nbs[6] = _c_(glm::vec3(l.x - 1.f, l.y + 1.f, l.z - 1.f)); nbs[7] = _c_(glm::vec3(l.x, l.y + 1.f, l.z - 1.f)); nbs[8] = _c_(glm::vec3(l.x + 1.f, l.y + 1.f, l.z - 1.f));

			nbs[9] = _c_(glm::vec3(l.x - 1.f, l.y - 1.f, l.z));  nbs[10] = _c_(glm::vec3(l.x, l.y - 1.f, l.z)); nbs[11] = _c_(glm::vec3(l.x + 1.f, l.y - 1.f, l.z));
			nbs[12] = _c_(glm::vec3(l.x - 1.f, l.y, l.z));     nbs[13] = _c_(glm::vec3(l.x, l.y, l.z));     nbs[14] = _c_(glm::vec3(l.x + 1.f, l.y, l.z));
			nbs[15] = _c_(glm::vec3(l.x - 1.f, l.y + 1.f, l.z)); nbs[16] = _c_(glm::vec3(l.x, l.y + 1.f, l.z)); nbs[17] = _c_(glm::vec3(l.x + 1.f, l.y + 1.f, l.z));

			nbs[18] = _c_(glm::vec3(l.x - 1.f, l.y - 1.f, l.z + 1.f)); nbs[19] = _c_(glm::vec3(l.x, l.y - 1.f, l.z + 1.f)); nbs[20] = _c_(glm::vec3(l.x + 1.f, l.y - 1.f, l.z + 1.f));
			nbs[21] = _c_(glm::vec3(l.x - 1.f, l.y, l.z + 1.f));     nbs[22] = _c_(glm::vec3(l.x, l.y, l.z + 1.f));     nbs[23] = _c_(glm::vec3(l.x + 1.f, l.y, l.z + 1.f));
			nbs[24] = _c_(glm::vec3(l.x - 1.f, l.y + 1.f, l.z + 1.f)); nbs[25] = _c_(glm::vec3(l.x, l.y + 1.f, l.z + 1.f)); nbs[26] = _c_(glm::vec3(l.x + 1.f, l.y + 1.f, l.z + 1.f));

			for (int i = 0; i < 27; i++) if (i != 0) if (nbs[0] != -1) if (nbs[0] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 1) if (nbs[1] != -1) if (nbs[1] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 2) if (nbs[2] != -1) if (nbs[2] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 3) if (nbs[3] != -1) if (nbs[3] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 4) if (nbs[4] != -1) if (nbs[4] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 5) if (nbs[5] != -1) if (nbs[5] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 6) if (nbs[6] != -1) if (nbs[6] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 7) if (nbs[7] != -1) if (nbs[7] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 8) if (nbs[8] != -1) if (nbs[8] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 9) if (nbs[9] != -1) if (nbs[9] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 10) if (nbs[10] != -1) if (nbs[10] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 11) if (nbs[11] != -1) if (nbs[11] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 12) if (nbs[12] != -1) if (nbs[12] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 13) if (nbs[13] != -1) if (nbs[13] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 14) if (nbs[14] != -1) if (nbs[14] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 15) if (nbs[15] != -1) if (nbs[15] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 16) if (nbs[16] != -1) if (nbs[16] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 17) if (nbs[17] != -1) if (nbs[17] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 18) if (nbs[18] != -1) if (nbs[18] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 19) if (nbs[19] != -1) if (nbs[19] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 20) if (nbs[20] != -1) if (nbs[20] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 21) if (nbs[21] != -1) if (nbs[21] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 22) if (nbs[22] != -1) if (nbs[22] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 23) if (nbs[23] != -1) if (nbs[23] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 24) if (nbs[24] != -1) if (nbs[24] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 25) if (nbs[25] != -1) if (nbs[25] == nbs[i]) nbs[i] = -1;

			for (int i = 0; i < 27; i++) if (i != 26) if (nbs[26] != -1) if (nbs[26] == nbs[i]) nbs[i] = -1;

			return nbs;
		}

		//string CompactSearch::referencesToString(){
		//	stringstream ss;
		//
		//	for( int i=0; i < particleCount; i++ ){
		//		ss << "[ id:" << references[i].particleID << ",\t gindex: " << references[i].gridIndex << ",\t head: " << references[i].head << " ]"<< endl;
		//	}
		//
		//	return ss.str();
		//}

		int  CompactSearch::findNeighboursInGridCell(Particle **particles, int particleCount, int inx, int cellid, Particle *particle) {

			int offsetCounter = 0;
			do {

				Particle *otherParticle = particles[references[inx].particleID];

				glm::vec3 differenceVector = particle->x - otherParticle->x;
				float vlength = glm::length(differenceVector);

				if (vlength < h) {

					Neighbour cache;

					cache.neighbour = otherParticle;

					if (particle->nbCount < neighbourListSize) {
						particle->nbs[particle->nbCount] = cache;
						particle->nbCount++;
					}
				}

				inx++;
				offsetCounter++;

			} while (references[inx].gridIndex == cellid && inx < particleCount);

			return offsetCounter;
		}


		void CompactSearch::findNeighbours(Particle **particles, int particleCount) {
			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < particleCount; i++) {

					if (particles[i]->nbs != NULL) {
						delete[] particles[i]->nbs;
						particles[i]->nbCount = 0;
					}

					particles[i]->nbs = new Neighbour[neighbourListSize];

					size_t *nbs = getNeighbourCells(particles[i]->x);

					for (int j = 0; j < 27; j++) {
						if (nbs[j] >= 0 && nbs[j] < cellHashMax) {
							if (gridMap[nbs[j]] != -1) {
								findNeighboursInGridCell(particles, particleCount, gridMap[nbs[j]], references[gridMap[nbs[j]]].gridIndex, particles[i]);
							}
						}
					}

					delete[] nbs;
				}
			}
		}

		void CompactSearch::findNeighbours(Particle **particles, int nbParticles, double _h, double _pradius) {

			h = _h;

			minMax(particles, nbParticles);

			float xlength = std::abs((max_x - min_x));// + h;
			float ylength = std::abs((max_y - min_x));// + h;
			float zlength = std::abs((max_z - min_x));// + h;

			size_t xcellCount = (size_t)(xlength / (h));
			size_t ycellCount = (size_t)(ylength / (h));
			size_t zcellCount = (size_t)(zlength / (h));

			cell_count = xcellCount * ycellCount*zcellCount;

			if (cell_count == 0) cell_count = 1;

			references = new GridReference[nbParticles];

			hashParticles(particles, nbParticles);

			sortAndIndexReferences(particles, nbParticles);

			findNeighbours(particles, nbParticles);

			delete[] references;
			delete[] gridMap;
			
		}
	}
}