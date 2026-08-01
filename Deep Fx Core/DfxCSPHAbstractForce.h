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

#include <DfxUtility.h>
#include "DfxCSimulationEntity.h"
#include "DfxCSPHParticle.h"

namespace dfxcore{

	namespace sph {
	
		class AbstractForce{
		protected:
			sph::Particle **particles;
			glm::vec3 *forces;
			size_t nbParticles;
			float *boundary_mass;
			float _mass;
			float _h;
			float _h2;

		public:
			virtual void addForce() = 0;

			virtual ~AbstractForce();

			void setParticles(sph::Particle **pts, size_t nbp);

			void setForces(glm::vec3 *fc);

			void setBoundaryMasses(float *bm);

			void setMass(float m);

			void setKernelSpacing(float kp);
		};
	}
}