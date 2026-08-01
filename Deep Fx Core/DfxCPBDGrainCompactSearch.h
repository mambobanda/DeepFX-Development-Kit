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
#include "DfxCPBDGrainParticle.h"


#define MAP_SPARSE 0
#define MAP_DENSE 1
#define MAP_BALANCED 2


namespace dfxcore {

	namespace pbd
	{
		typedef struct {
			size_t particleID;
			size_t gridIndex;
			size_t head;
		}GridReference;

		class GridCell {
		public:

			size_t GridReference;
			size_t *ids;
			size_t count;

			GridCell();
		};

		class CompactSearch {
		private:
			float min_x;
			float min_y;
			float min_z;

			float max_x;
			float max_y;
			float max_z;

			size_t   cellHashMax;

			float h;

			size_t cell_count;
			size_t neighbourListSize;

			GridReference *references;
			int           *gridMap;

			//Particle **particles;
			//int      particleCount;

			glm::vec3 getSpatialLocation(glm::vec3 p);
			size_t getSpatialHash(glm::vec3 p);

			int  findMaxGridCellValue(Particle **particles, int particleCount);
			void minMax(Particle **particles, int particleCount);
			void hashParticles(Particle **particles, int particleCount);
			void sortAndIndexReferences(Particle **particles, int particleCount);
			void findNeighbours(Particle **particles, int particleCount);

			int  findNeighboursInGridCell(Particle **particles, int particleCount, int inx, int cellid, Particle *);
			size_t *getNeighbourCells(glm::vec3 position);
			static bool referenceSort(GridReference, GridReference);

			size_t _c_(glm::vec3 position);
		public:
			CompactSearch(int neighbourListSize = 47);
			void search();

			void findNeighbours(Particle **particles, int nbParticles, double _h, double _pradius);

			std::string referencesToString();
		};
	}
}