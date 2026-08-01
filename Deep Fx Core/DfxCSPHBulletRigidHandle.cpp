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

#include "DfxCSPHBulletRigidHandle.h"

namespace dfxcore {

	namespace sph {

		DfxCSPHBulletRigidHandle::DfxCSPHBulletRigidHandle() {
			handlePtr = nullptr;
			couplingType = KINEMATIC;
		}

		void DfxCSPHBulletRigidHandle::setBulletHandle(bullet::DfxCBulletRigidHandle *handlePtr) {
			this->handlePtr = handlePtr;
		}

		bool DfxCSPHBulletRigidHandle::isDynamic() {
			if (couplingType == DYNAMIC) return true;
			else return false;
		}

		void DfxCSPHBulletRigidHandle::makeKinematic() {
			couplingType = KINEMATIC;
		}

		void DfxCSPHBulletRigidHandle::makeDynamic() {
			couplingType = DYNAMIC;
		}

		bullet::DfxCBulletRigidHandle *DfxCSPHBulletRigidHandle::getBulletHandle() {
			return handlePtr;
		}

		void DfxCSPHBulletRigidHandle::sampleMesh(float voxSize) {
			points.clear();

			if (handlePtr != nullptr) {

				if (handlePtr->getMeshDataPtr() != nullptr) {
					auto mesh = handlePtr->getMeshDataPtr();

					if (mesh->getVertexCount() > 3 && ((int)(mesh->getIndexCount() / 3)) >= 1) {

						DfxGlobal::info::message("Voxelizing SPH Bullet Rigid...");
						
						glm::vec3 scale(handlePtr->getInitialScale());
						voxelizeMesh(*mesh, scale, points, voxSize);

						DfxGlobal::info::message("");
					}
				}
			}
		}

		void DfxCSPHBulletRigidHandle::initSPHParticles() {
			clearParticles();

			if (points.size() > 0) {

				for (int i = 0; i < points.size(); i++) {
					Particle *p = new Particle(BOUNDARY_PARTICLE);
					p->x = points[i];
					p->setAlive(true);

					particles.push_back(p);
				}

				DfxGlobal::console::println("Voxel Count: " + std::to_string(points.size()) + "\n");

			}
		}

		void DfxCSPHBulletRigidHandle::applyInitialTransform() {
			if (handlePtr != nullptr) {

				auto mat = handlePtr->worldTransform();

				for (size_t i = 0; i < particles.size(); i++) {
					particles[i]->x = glm::vec3(mat * glm::vec4(points[i], 1.0f));
				}
			}
		}

		size_t DfxCSPHBulletRigidHandle::particleCount() {
			return particles.size();
		}

		Particle *DfxCSPHBulletRigidHandle::getParticle(size_t index) {
			return particles[index];
		}

		void DfxCSPHBulletRigidHandle::updateBulletTransform() {
			if (handlePtr != nullptr) {

				if (handlePtr->getMass() > 0.f) {
					auto mat = handlePtr->getBulletTransformNoScale();

					for (size_t i = 0; i < particles.size(); i++) {
						particles[i]->x = glm::vec3(mat * glm::vec4(points[i], 1.0f));
					}
				}
			}
		}

		void DfxCSPHBulletRigidHandle::clearParticles() {
			for (auto p : particles) {
				delete p;
			}

			particles.clear();
		}
	}
}