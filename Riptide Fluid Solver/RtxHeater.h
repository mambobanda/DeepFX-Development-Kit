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

#include <glm/vec3.hpp>
#include "RtxCommon.h"

namespace rtx {
	class Heater {
	private:
		float _radius;
		glm::vec3 _position;
		int _id;
	public:

		Heater();
		virtual ~Heater();

		int index();
		void setIndex(int val);

		float radius();
		void setRadius(float val);

		glm::vec3 position();
		void setPosition(glm::vec3 val);

		void heat(size_t nbParticles, int8_t *alive, int8_t *frozen, int8_t *type, glm::vec3 *positions, float *viscosity);
	};
}