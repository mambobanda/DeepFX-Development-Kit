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


#include "DfxCPBDGrainParticle.h"

namespace dfxcore {

	namespace pbd {

		Particle::Particle(int typ) {

			type = typ;
			density = 0.0f;
			v = glm::vec3(0.0f);
			x = glm::vec3(0.0f);

			alive = true;
			nbCount = 0;
			nbs = NULL;
		}

		Particle::~Particle() {
			if (nbs != NULL) delete[] nbs;
		}

		int Particle::getType() {
			return type;
		}

		void Particle::setIndex(int i) {
			index = i;
		}

		int Particle::getIndex() {
			return index;
		}

		void Particle::setAlive(bool al) {
			alive = al;
		}

		bool Particle::isAlive() {
			return alive;
		}
	}
}