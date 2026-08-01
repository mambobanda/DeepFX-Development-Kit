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

#include "RtxCompactXSearch.h"
#include "RtxSPHSystem.h"

namespace rtx {
	
	CompactXSearch::CompactXSearch() {
		_search = nullptr;
		_sphSystem = nullptr;
	}

	CompactXSearch::~CompactXSearch() {

	}

	void CompactXSearch::reset(SPHSystem *sphSystem) {

		if (_search != nullptr) delete _search;
		_search = nullptr;
		_sphSystem = nullptr;

		if (sphSystem != nullptr) {
			_search = new CompactNSearch::NeighborhoodSearch(1.f, true);
			_sphSystem = sphSystem;

			_search->add_point_set((float*)sphSystem->positions(), sphSystem->nbParticles(), false, true);
		}
	}

	void CompactXSearch::setRadius(float r) {
		if (_search != nullptr) {
			_search->set_radius(r);
		}
	}

	void CompactXSearch::findNeighbours() {
		if (_search != nullptr) {
			_search->find_neighbors();

			_sphSystem->clearNeighbourList();

			CompactNSearch::PointSet const& ps = _search->point_set(0);
			for (int i = 0; i < ps.n_points(); ++i)
			{
				int counter = 0;
				for (unsigned int j = 0; j < ps.n_neighbors(0, i); ++j){
					
					const unsigned int pid = ps.neighbor(0, i, j);
				
					auto nbListSize = _sphSystem->nbListSize();
					auto nbListOffset = i * nbListSize;
					if (counter < nbListSize) {
						_sphSystem->neighbours()[nbListOffset + counter] = pid;
						_sphSystem->nbCount()[i] = ++counter;
					}
					else {
						break;
					}
				}
			}
		}
	}

	void CompactXSearch::updatePositions() {
		if (_search != nullptr) {
			_search->update_point_sets();
		}
	}

}