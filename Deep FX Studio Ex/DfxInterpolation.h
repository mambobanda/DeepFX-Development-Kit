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

#include <glm/vec2.hpp>
#include <algorithm>

namespace util {
	inline glm::vec2 cubicBezierInterpolation(float t, glm::vec2 p1, glm::vec2 p2, glm::vec2 c1, glm::vec2 c2) {
		return (std::powf(1.f - t, 3) * p1) + (3.f * std::powf(1.f - t, 2) * t * c1) + (3.f * (1.f - t) * std::powf(t, 2.f) * c2) + (std::powf(t, 3.f) * p2);
	}
}