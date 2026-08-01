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


#include <DfxGlobal.h>
#include <DfxCMeshVoxelisingFunctions.h>
#include "DfxCBulletRigidHandle.h"
#include "DfxCSPHParticle.h"

namespace dfxcore {

	namespace sph {

		class DfxCSPHBulletRigidHandle {
		private:
			bullet::DfxCBulletRigidHandle *handlePtr;
			std::vector<Particle*> particles;
			std::vector<glm::vec3> points;

			size_t couplingType;
		public:
			enum {KINEMATIC=0xff, DYNAMIC};

			DfxCSPHBulletRigidHandle();

			void setBulletHandle(bullet::DfxCBulletRigidHandle *handlePtr);
			bullet::DfxCBulletRigidHandle *getBulletHandle();

			void sampleMesh(float voxelSize);
			void initSPHParticles();
			void applyInitialTransform();

			size_t particleCount();
			Particle *getParticle(size_t index);

			void updateBulletTransform();

			void clearParticles();

			bool isDynamic();
			void makeKinematic();
			void makeDynamic();
		};
	}
}