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

#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>
#include "Util.h"

namespace dfxcore {

	namespace sph {

		static float poly6_radius;
		static float poly6_k;
		static float poly6_l;
		static float poly6_m;

		static void initPoly6Kernel(float _h) {
			poly6_radius = _h;
			static const float pi = static_cast<float>(xPI);
			poly6_k = 315.0  / (64.0*pi*pow9(poly6_radius));
			poly6_l = -945.0 / (32.0*pi*pow9(poly6_radius));
			poly6_m = poly6_l;
		}

		static float poly6W(glm::vec3 &r) {

			float res = 0.0f;

			const float rl = glm::length(r);
			const float r2 = rl*rl;
			const float radius2 = poly6_radius*poly6_radius;


			if (r2 <= radius2)
			{
				res = pow(radius2 - r2, 3)*poly6_k;
			}

			return res;
		}

		static glm::vec3 poly6GradW(glm::vec3 &r) {
			glm::vec3 res(0.0f);

			const float rl = glm::length(r);
			const float r2 = rl*rl;
			const float radius2 = poly6_radius*poly6_radius;

			if (r2 <= radius2)
			{
				float tmp = radius2 - r2;
				res = poly6_l * tmp * tmp*r;
			}
			else
				return glm::vec3(0.f);

			return res;
		}
	}
}
