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
#include <algorithm>
#include "DfxCPBDGrainFrictionConstraint.h"


namespace dfxcore {

	namespace pbd {
		FrictionConstraint::FrictionConstraint() {

		}

		FrictionConstraint::FrictionConstraint(float kf, float sf, float rd, float im) {
			kineticFriction = kf;
			staticFriciton = sf;
			restDistance = rd;
			imass = im;
		}

		FrictionConstraint::~FrictionConstraint() {
		}

		void FrictionConstraint::solveConstraint(Particle **particles, Particle *particle, glm::vec3 *xi) {

			if (particle->getType() != GRANULAR_PARTICLE) return;

			for (int j = 0; j < particle->nbCount; j++) {
				Particle *np = particle->nbs[j].neighbour;

				if (!np->isAlive()) continue;
				if (particle->getIndex() == np->getIndex()) continue;

				glm::vec3  r = particle->x - np->x;
				float r_length = glm::length(r);

				if (particle->nbs[j].interDistance >= 0) continue;

				float distDiff = abs(particle->nbs[j].interDistance);

				glm::vec3 n = r / r_length;
				glm::vec3 xxi = particle->x - xi[particle->getIndex()];
				glm::vec3 xxj = np->x - xi[np->getIndex()];

				glm::vec3 tangentialDisplacement = (xxi - xxj) - (glm::dot(xxi - xxj, n) * n);
				float td_length = glm::length(tangentialDisplacement);

				float genMass = (imass / (imass + imass));

				if (td_length < (staticFriciton * distDiff)) {
					if (staticFriciton > 0.f) {

						if (np->getType() == GRANULAR_PARTICLE) {
							particle->x -= genMass * tangentialDisplacement;
							np->x -= -genMass * tangentialDisplacement;
						}
						else {

						}
					}
				}
				else {
					if (kineticFriction > 0.f) {

						float upper = kineticFriction * distDiff;
						float ut = upper / td_length;
						glm::vec3 dp = tangentialDisplacement * std::min(upper / td_length, 1.f);

						if (np->getType() == GRANULAR_PARTICLE) {
							particle->x -= genMass * dp;
							np->x -= -genMass * dp;
						}
						else {

						}
					}
				}

			}
		}
	}
}