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

#include <list>
#include <glm\vec3.hpp>

namespace dfxcore {

	class FoamParticle {
	public:
		float life;
		glm::vec3 pos;
		glm::vec3 last_pos;
		glm::vec3 vel;

		std::list<size_t> nbs;
	};
}