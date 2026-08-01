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

#include "DfxCPBDGrainPenetrationConstraint.h"

namespace dfxcore {

	namespace pbd {

		PenetrationConstraint::PenetrationConstraint() {

		}

		PenetrationConstraint::PenetrationConstraint(float r, float _k, float i) {
			restDistance = r;
			k = _k;
			imass = i;
		}


		PenetrationConstraint::~PenetrationConstraint() {

		}

		void PenetrationConstraint::solveConstraint(Particle **particles, Particle *particle, glm::vec3 *xi) {

			if (particle->getType() != GRANULAR_PARTICLE) return;

			for (int j = 0; j < particle->nbCount; j++) {
				Particle *np = particle->nbs[j].neighbour;

				if (!np->isAlive()) continue;
				if (particle->getIndex() == np->getIndex()) continue;

				glm::vec3  r = particle->x - np->x;
				float r_length = glm::length(r);

				float distDiff = r_length - restDistance;

				if (distDiff >= 0) continue;

				glm::vec3 derivative = r * (1.0f / r_length);

				float genMass = (imass / (imass + imass));

				glm::vec3 dP1 = genMass * ((derivative) * (distDiff * -0.4f));

				particle->x += dP1 * k;

				if (np->getType() == GRANULAR_PARTICLE)
					np->x += -dP1 * k;
			}
		}
	}
}