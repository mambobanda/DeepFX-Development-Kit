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

#include "DfxCSPHAbstractForce.h"


namespace dfxcore {

	namespace sph {

		AbstractForce::~AbstractForce() {
		}

		void AbstractForce::setParticles(sph::Particle **pts, size_t nbp) {
			particles = pts;
			nbParticles = nbp;
		}

		void AbstractForce::setForces(glm::vec3 *fc) {
			forces = fc;
		}

		void AbstractForce::setBoundaryMasses(float *bm) {
			boundary_mass = bm;
		}

		void AbstractForce::setMass(float m) {
			_mass = m;
		}

		void AbstractForce::setKernelSpacing(float kp) {
			_h  = kp;
			_h2 = kp * kp;
		}
	}
}