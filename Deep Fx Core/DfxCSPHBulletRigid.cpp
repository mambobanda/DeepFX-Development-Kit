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

#include "DfxCBulletRigidObject.h"
#include "DfxCSPHBulletRigid.h"

namespace dfxcore {

	namespace sph {

		SPHBulletRigid::SPHBulletRigid(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
			setLogicalName("SPH Bullet Rigid");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
			uid->setRootID(getInstanceID());

			setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inShowParticles, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inCouplingType, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inVoxelSize, DfxEntityPropertyState::INPUT_STATE);

			((DfxEnum*)getProperty(inCouplingType))->add("Dynamic");
			((DfxEnum*)getProperty(inCouplingType))->add("Kinematic");

			((DfxEnum*)getProperty(inShowParticles))->add("No");
			((DfxEnum*)getProperty(inShowParticles))->add("Yes");

			*((float*)getProperty(inVoxelSize)) = 0.045f;
			
			getDataInstance()->makeDirty();
		}

		SPHBulletRigid::~SPHBulletRigid() {
			for (auto h : rigidHandles) {
				h->clearParticles();
				delete h;
			}
		}

		void SPHBulletRigid::compute() {
			using namespace DfxEntityPropertyTypes;

		}

		std::vector<DfxCSPHBulletRigidHandle*> &SPHBulletRigid::getRigidHandles() {
			return rigidHandles;
		}

		void SPHBulletRigid::preInit() {
					
		}

		void SPHBulletRigid::init() {
			using namespace DfxEntityPropertyTypes;

			if (propertyHasBinding(inRef)) {
				pingParents();			

				if (!getDataInstance()->isClean()) {
					DfxGlobal::console::println("Dirty SPH Bullet Rigid...");

					for (auto h : rigidHandles) {
						h->clearParticles();
						delete h;
					}

					rigidHandles.clear();
				}			

				float vsize = *(float*)getProperty(inVoxelSize);
				bool isDynamic = ((DfxEnum*)getProperty(inCouplingType))->getCurrentIndex() == 0 ? true : false;

				DfxEntityPropertyTypes::UID *in_uid = (DfxEntityPropertyTypes::UID*)getProperty(inRef);
				auto sims = findSimulationEntities(in_uid);

				for (auto s : sims) {

					if (s->UniqueName().compare("2F2X252X_DFX_BULLET_RIGID_OBJECT") == 0) {

						auto bObjPtr = (bullet::DfxCBulletRigidObject*)s;
						size_t handlerCount = bObjPtr->getRigidHandles().size();

						if (handlerCount != rigidHandles.size()) {
							for (auto h : rigidHandles) {
								h->clearParticles();
								delete h;
							}

							rigidHandles.clear();
						}

						for (size_t i = 0; i < handlerCount; i++) {
							auto h = bObjPtr->getRigidHandles()[i];

							if (h->getMeshDataPtr() != nullptr) {

								if (h->getMeshDataPtr()->getIndexCount() >= 0) {

									DfxCSPHBulletRigidHandle *sph_h = nullptr;

									if (!getDataInstance()->isClean() || rigidHandles.size() == 0) {
										auto sph_h = new DfxCSPHBulletRigidHandle();
										sph_h->setBulletHandle(h);
										sph_h->sampleMesh(vsize);
										sph_h->initSPHParticles();
										sph_h->applyInitialTransform();

										if (isDynamic) sph_h->makeDynamic();
										else sph_h->makeKinematic();

										rigidHandles.push_back(sph_h);
									}
									else {
										sph_h = rigidHandles[i];
										sph_h->setBulletHandle(h);
										sph_h->initSPHParticles();
										sph_h->applyInitialTransform();

										if (isDynamic) sph_h->makeDynamic();
										else sph_h->makeKinematic();
									}
									
								}
							}
						}
					}
				}

				getDataInstance()->makeClean();
				makeChildrenDirty();
			}
		}

		void SPHBulletRigid::draw() {
			using namespace DfxEntityPropertyTypes;

			initializeOpenGLFunctions();

			bool showParticles = ((DfxEnum*)getProperty(inShowParticles))->getCurrentIndex() == 0 ? false : true;

			if (showParticles) {
				if (rigidHandles.size() > 0) {

					for (auto handle : rigidHandles) {

						if (handle->particleCount() > 0) {
							for (size_t inx = 0; inx < handle->particleCount(); inx++) {

								if (handle->getParticle(inx) != nullptr) {

									glColor3f(0.f, 1.f, 0);
									glPointSize(3);
									glBegin(GL_POINTS);
									glVertex3f(handle->getParticle(inx)->x.x, handle->getParticle(inx)->x.y, handle->getParticle(inx)->x.z);
									glEnd();
									glPointSize(1);
								}
							}
						}
					}
				}
			}
		}

		void SPHBulletRigid::clear() {

		}

		void SPHBulletRigid::done() {
			for (auto h : rigidHandles) {
				h->clearParticles();
			}
		}

		void SPHBulletRigid::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHBulletRigid::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHBulletRigid::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHBulletRigid::isExportable() {
			return true;
		}

		void *SPHBulletRigid::creator(DfxGlobalEntityName uniquename) {
			return new SPHBulletRigid(uniquename);
		}

		void SPHBulletRigid::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inCouplingType, "coupling", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inVoxelSize, "voxel_size", new DfxEntityPropertyTypes::FloatPropertyType());	
			DfxEntityDB::createProperty(inShowParticles, "show_particles", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		}
	}
}