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

#pragma once

#include <glm\vec3.hpp>
#include "DfxCSPHParticleType.h"

namespace dfxcore {

	namespace sph {
		class Particle;

		class Particle {
		private:
			int type;
			int index;
			bool alive;
		public:
			Particle(int typ);
			~Particle();

			float density;
			glm::vec3  v;
			glm::vec3  x;

			int nbCount;
			unsigned int *nbIndices;
			//Neighbour *nbs;

			/*int nbnbCount;
			unsigned int *nbnbIndices;*/
			//Neighbour *nbnbs;

			int getType();

			void setIndex(int i);
			int getIndex();

			void setAlive(bool al);
			bool isAlive();
		};
	}
}
