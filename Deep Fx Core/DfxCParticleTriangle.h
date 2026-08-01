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

#include <glm/vec3.hpp>

namespace dfxcore {

	class DfxCParticleTriangle {
	public:
		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 c;

		glm::vec3 normal;
	};
};
