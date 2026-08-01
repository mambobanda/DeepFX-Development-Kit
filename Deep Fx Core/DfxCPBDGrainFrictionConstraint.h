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

#include <glm/geometric.hpp>
#include "DfxPBDGrainParticleType.h"
#include "DfxCPBDGrainConstraint.h"

namespace dfxcore {

	namespace pbd {
		class FrictionConstraint : public Constraint {
		private:
			float kineticFriction;
			float staticFriciton;
			float restDistance;
			float imass;
		public:
			FrictionConstraint();
			FrictionConstraint(float kf, float sf, float rd, float im);
			virtual ~FrictionConstraint();

			virtual void solveConstraint(Particle **particles, Particle *particle, glm::vec3 *xi);
		};
	}
}