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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <vector>
#include "SgCommon.h"

namespace deepsg {
	template<typename T>
	bool vector_move(std::vector<T> &vec, size_t source, size_t destination) {

		if (vec.size() > 1) {
			if (source >= 0 && source < vec.size()) {
				auto itr = vec.begin() + source;

				if (itr != vec.end()) {
					auto val = (*itr);
					vec.erase(itr);

					size_t destInx = 0;

					if (destination > source) destInx = destination - 1;

					if (destination >= 0 || destination >= vec.size()) {
						if (destination >= vec.size()) {
							vec.push_back(val);
							return true;
						}
						else {
							vec.insert(vec.begin() + destination, val);
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	void evaluateSgNodeFilters(SgRootNode *rootNode);
}