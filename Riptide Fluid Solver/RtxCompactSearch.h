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
#include <algorithm>
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include "RtxAABB.h"
#include "RtxSpatialHash.h"
#include "RtxNeighbourSearch.h"

namespace rtx {

	class CompactSearch : public RtxNeighbourSearch {
	private:
		int neighbourBufferSize;
	public:
		CompactSearch();

		void findNeighbours(size_t nbParticles, int8_t nbListSize, float h, float radius, glm::vec3 *positions, int8_t *alive, int8_t *type,
			int8_t *nbCount, size_t *neighbours) override;

		int getNeighbourBufferSize();
		void setNeighbourBufferSize(int);

		glm::vec3 *getNeighbourCells(glm::vec3 position, float, float, float);

		void findNeighboursInCell(std::vector<size_t> &particles, size_t p, int &counter, int8_t nbListSize, float h, glm::vec3 *positions, int8_t *alive, int8_t *type,
			int8_t *nbCount, size_t *neighbours);

		template<typename T>
		void neighbour_search(size_t nbParticles, int8_t nbListSize, float h, float radius, T compact_hash, glm::vec3 *positions, int8_t *alive, int8_t *type,
			int8_t *nbCount, size_t *neighbours) {

			AABB bbox;

			bbox.empty();

			for (int i = 0; i < nbParticles; i++)
				bbox.add(positions[i].x, positions[i].y, positions[i].z);

			bbox.addPadding(radius, radius, radius);

			float xlength = std::abs((bbox.xmax - bbox.xmin)) + h;
			float ylength = std::abs((bbox.ymax - bbox.ymin)) + h;
			float zlength = std::abs((bbox.zmax - bbox.zmin)) + h;

			int xcellCount = (int)std::max(1.0f, (xlength / h));
			int ycellCount = (int)std::max(1.0f, (ylength / h));
			int zcellCount = (int)std::max(1.0f, (zlength / h));

			int totalCellCount = xcellCount * ycellCount * zcellCount;

			setSpatialHashAttributes(h, totalCellCount);

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


			for (size_t i = 0; i < nbParticles; i++) {
				if (!alive[i]) continue;

				glm::vec3 location = getSpatialLocation(positions[i], bbox.xmin, bbox.ymin, bbox.zmin);

				auto it = compact_hash.find(location);

				if (it != compact_hash.end()) {

					if (alive[i]) {
						it->second.push_back(i);
					}
				}
				else {

					if (alive[i]) {
						std::vector<size_t> cell;
						cell.reserve(10);
						cell.push_back(i);
						compact_hash[location] = cell;
					}
				}
			}

			size_t list_size = compact_hash.size();
			std::vector<size_t> *cell_list = new std::vector<size_t>[list_size];

			int list_counter = 0;
			auto list_it = compact_hash.begin();
			list_it = compact_hash.begin();
			for (; list_it != compact_hash.end(); list_it++) {
				cell_list[list_counter] = list_it->second;
				list_counter++;
			}

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < list_size; i++) {

					std::vector<size_t> c = cell_list[i];
			
					for (int jnx = 0; jnx < c.size(); jnx++) {

						// Local Variables
						size_t p = c[jnx];

						int counter = 0;
						int nbcounter = 0;

						// Find Neighbour Particles /////////////////////////////////////////////////////////////////
						glm::vec3 *nb_locations = getNeighbourCells(positions[p], bbox.xmin, bbox.ymin, bbox.zmin);

						for (int inx = 0; inx < 27; inx++) {

							auto it = compact_hash.find(nb_locations[inx]);

							if (it != compact_hash.end()) {
								findNeighboursInCell(it->second, p, counter, nbListSize, h, positions, alive, type, nbCount, neighbours);
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
	};

}