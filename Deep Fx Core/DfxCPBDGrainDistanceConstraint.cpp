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

#include <iostream>
#include "DfxCPBDGrainDistanceConstraint.h"

using namespace std;

namespace dfxcore {

	namespace pbd {

		DistanceConstraint::DistanceConstraint() {

		}

		DistanceConstraint::DistanceConstraint(Particle **_particles, std::vector<size_t> *_nlist, float rd, float im) {
			restDistance = rd + 0.02;
			imass = im;
			nlist = _nlist;
			particles = _particles;
		}

		DistanceConstraint::~DistanceConstraint() {
		}

		void DistanceConstraint::solveConstraint(Particle **particles, Particle *particle, glm::vec3 *xi) {

			if (particle->getType() != GRANULAR_PARTICLE) return;

			for (int j = 0; j < nlist[particle->getIndex()].size(); j++) {

				Particle *np = particles[nlist[particle->getIndex()][j]];

				if (!np->isAlive()) continue;
				if (np->getType() != GRANULAR_PARTICLE) return;
				if (particle->getIndex() == np->getIndex()) continue;

				glm::vec3  r = particle->x - np->x;
				glm::vec3  n = glm::normalize(r);
				float distance = glm::length(r);

				float genMass = (imass / (imass + imass));

				glm::vec3 corr(0);
				if (restDistance - distance < 0) {
					corr = (distance - restDistance) * n;

					particle->x -= .5f * genMass * corr;
					np->x -= .5f * -genMass * corr;
				}
				else {

				}

			}
		}
	}
}