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

#include "DfxCSPHParticleCache.h"
#include "DfxSimulationCoreGlobal.h"

namespace dfxcore {

	namespace sph {

		SPHParticleCache::SPHParticleCache(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_POINT_ITEM) {
			setLogicalName("SPH Particle Cache");

			setPropertyState(inFrame, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inParticleSize, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inSource, DfxEntityPropertyState::INPUT_STATE);

			*(float*)getProperty(inFrame) = 0;
			*(float*)getProperty(inParticleSize) = 4.0f;

			((DfxEnum*)getProperty(inType))->add("Liquid Partciles");
			((DfxEnum*)getProperty(inType))->add("Boundary Particles");
			((DfxEnum*)getProperty(inType))->setCurrentIndex(0);

			// DfxScene::getInstance()->addItem(this);
		}

		SPHParticleCache::~SPHParticleCache() {
			clearSimulationCache();
			// DfxScene::getInstance()->removeItemByID(this->getInstanceID());
		}

		void SPHParticleCache::init() {

		}

		void SPHParticleCache::clear() {

		}

		void SPHParticleCache::load(std::ifstream &) {
			*((size_t*)getProperty(item_id)) = getInstanceID();
		}

		void SPHParticleCache::done() {
			clearSimulationCache();
		}

		void SPHParticleCache::reset() {
			
		}

		void SPHParticleCache::compute() {

			if (propertyHasBinding(inSource)) {

				size_t inst_id = *(size_t*)getProperty(inSource);
				
				if (DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, DivergenceFreeSPHSolver_Identifier)) {

					cacheDFSPHNode();
				}else if(DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, SPHLiquid_Identifier)) {

					cacheSPHLiquid();
				}else if (DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, SPHRigid_Identifier)) {

					cacheSPHRigid();
				}

			}

			cleanAllProperties();
			getDataInstance()->makeClean();
		}

		void SPHParticleCache::cacheDFSPHNode() {
			
			int frame_num = *(float*)getProperty(inFrame);
			int type  = ((DfxEnum*)getProperty(inType))->getCurrentIndex();
			size_t inst_id = *(size_t*)getProperty(inSource);

			DivergenceFreeSPHSolver *ent = (DivergenceFreeSPHSolver*)DfxSimulationCoreGlobal::getInstance()->findSimulationEntity(inst_id);

			if (ent != nullptr) {
				
				DfxGlobal::info::message("Caching SPH Particles...");

				if (ent->getParticleCount() > 0) {

					size_t count = 0;

					for (int inx = 0; inx < ent->getParticleCount(); inx++) {

						if (type == 0) {

							if(ent->getParticle(inx)->getType() == FLUID_PARTICLE) count++;
						}
						else if (type == 1) {

							if (ent->getParticle(inx)->getType() == BOUNDARY_PARTICLE) count++;
						}
					}
					
					if (getFrame(frame_num) == nullptr) {

						SimulationFrame *frame = new SimulationFrame();
						frame->initFrame(count);

						count = 0;
						for (int inx = 0; inx < ent->getParticleCount(); inx++) {
							
							if (type == 0) {

								if (ent->getParticle(inx)->getType() == FLUID_PARTICLE) {

									frame->getParticles()[count]  = ent->getParticle(inx)->x;
									frame->getSpeeds()[count]     =  glm::length(ent->getParticle(inx)->v);
									frame->getVelocities()[count] = ent->getParticle(inx)->v;
									count++;
								}
							}
							else if (type == 1) {

								if (ent->getParticle(inx)->getType() == BOUNDARY_PARTICLE) {

									frame->getParticles()[count] = ent->getParticle(inx)->x;
									frame->getSpeeds()[count] = 0.f;
									frame->getVelocities()[count] = glm::vec3(0.f);
									count++;
								}
							}
						}

						frame->setIsCached(true);
						insertFrame(frame_num, frame);
					}
				}
			}
			DfxGlobal::info::message("");
		}

		void SPHParticleCache::cacheSPHLiquid() {

		}

		void SPHParticleCache::cacheSPHRigid() {

		}

		void SPHParticleCache::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHParticleCache::draw() {

			glm::vec3 primary(0.f, 0.f, 1.0);
			glm::vec3 secondary(.85f, .85f, .85f);

			initializeOpenGLFunctions();
			int frame_num = *(float*)getProperty(inFrame);
			float psize = *(float*)getProperty(inParticleSize);
			
			if (getFrameCount() > 0) {
				
				SimulationFrame *frame = getFrame(frame_num);	
				if (frame == nullptr) frame = getFrame(frame_num - 1);
				
				if (frame != nullptr) {

					if (frame->isCached()) {
						glDisable(GL_LIGHTING);
						glPointSize(psize);
						glColor3f(0.f, 1.0f, 0.f);
						for (int i = 0; i < frame->getParticleCount(); i++) {

							glm::vec3 pos = frame->getParticles()[i];
							float speed = frame->getSpeeds()[i];

							glm::vec3 custom;
							getPreviewColor(speed, 10.0f, primary, secondary, custom);

							glBegin(GL_POINTS);
								glColor3f(custom[0], custom[1], custom[2]);
								glVertex3f(pos.x, pos.y, pos.z);
							glEnd();
						}
						glEnable(GL_LIGHTING);
					}
				}
			}
		}

		DfxBounds SPHParticleCache::bounds() {
			return DfxBounds();
		}

		bool SPHParticleCache::selectable() {
			return false;
		}

		void SPHParticleCache::calculateBounds() {
			//
		}

		void *SPHParticleCache::creator(DfxGlobalEntityName uniquename) {
			return new SPHParticleCache(uniquename);
		}

		void SPHParticleCache::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(inFrame, "frame", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inParticleSize, "particle_size", new DfxEntityPropertyTypes::FloatPropertyType);
			DfxEntityDB::createProperty(inType,"type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
			DfxEntityDB::createProperty(inSource, "source_id", new DfxEntityPropertyTypes::Size_TPropertyType());
		}

		int SPHParticleCache::getFrameCount() {
			return frames.size();
		}

		void SPHParticleCache::insertFrame(int frame, SimulationFrame *sframe) {

			std::pair<std::string, SimulationFrame*> p(indexToKey(frame), sframe);
			frames.insert(p);
		}

		SimulationFrame* SPHParticleCache::getFrame(int index) {
			std::unordered_map<std::string, SimulationFrame*>::iterator fr = frames.find(indexToKey(index));

			if (fr == frames.end()) {
				return nullptr;
			}
			else {
				SimulationFrame* ptr = fr->second;

				return ptr;
			}

		}

		void SPHParticleCache::clearSimulationCache() {
			std::unordered_map<std::string, SimulationFrame*>::iterator it = frames.begin();

			for (; it != frames.end(); it++) {
				it->second->clear();
				delete it->second;
				it->second = nullptr;
			}

			frames.clear();
		}

		SimulationFrame::SimulationFrame() {
			particles = nullptr;
			speeds = nullptr;
			velocities = nullptr;
			cached = false;
			particleCount = 0;
			frameHasSpeed = false;
			frameHasVelocity = false;
		}

		bool SimulationFrame::isCached() {
			return cached;
		}

		void SimulationFrame::setIsCached(bool cch) {
			cached = cch;
		}

		void SimulationFrame::clear() {
			delete[] particles;
			delete[] speeds;
			delete[] velocities;
			particleCount = 0;

			particles = nullptr;
			speeds = nullptr;
			velocities = nullptr;
		}

		void SimulationFrame::initFrame(int count) {
			particleCount = count;
			particles = new glm::vec3[count];
			velocities = new glm::vec3[count];
			speeds = new float[count];
		}

		glm::vec3 *SimulationFrame::getParticles() {
			return particles;
		}

		float *SimulationFrame::getSpeeds() {
			return speeds;
		}

		glm::vec3  *SimulationFrame::getVelocities() {
			return velocities;
		}

		void SimulationFrame::setHasSpeed(bool b) {
			frameHasSpeed = b;
		}

		void SimulationFrame::setHasVelocity(bool b) {
			frameHasVelocity = b;
		}

		bool SimulationFrame::hasSpeed() {
			return frameHasSpeed;
		}

		bool SimulationFrame::hasVelocity() {
			return frameHasVelocity;
		}

		int SimulationFrame::getParticleCount() {
			return particleCount;
		}

	}
}