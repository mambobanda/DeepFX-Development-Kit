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

#include "DfxCParticleCache.h"

namespace dfxcore {

	ParticleCache::ParticleCache(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
		setLogicalName("Particle Cache");

		setPropertyState(inType, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inDirPath, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPrefix, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inParticleSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPositionArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inVelocityArray, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inType))->add("Memory");
		((DfxEnum*)getProperty(inType))->add("Disk");

		*(float*)getProperty(inIndex) = 0.0f;
		*(float*)getProperty(inParticleSize) = 3.0f;

		*(std::string*)getProperty(inDirPath) = "c:/";
		*(std::string*)getProperty(inPrefix) = "default";
	}

	ParticleCache::~ParticleCache() {
		
	}

	void ParticleCache::init() {

	}

	void ParticleCache::clear() {

	}

	void ParticleCache::done() {
		memCache.clear();
	}

	void ParticleCache::compute() {
		using namespace DfxEntityPropertyTypes;
		
		int type = ((DfxEnum*)getProperty(inType))->getCurrentIndex();

		if (type == 0) {
			cacheToMemory();
		}
		else if (type == 1) {
			cacheToDisk();
		}

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void ParticleCache::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void ParticleCache::draw() {
		initializeOpenGLFunctions();

		if (memCache.getFrameCount() > 0) {

			int index = (int)*(float*)getProperty(inIndex);
			float psize = *(float*)getProperty(inParticleSize);

			SimulationFrame *frame = memCache.getFrame(index);
			if (frame == nullptr) frame = memCache.getFrame(index - 1);

			glm::vec3 primary(0.f, 0.f, 1.0);
			glm::vec3 secondary(.85f, .85f, .85f);

			if (frame != nullptr) {
				if (frame->isCached()) {

					glDisable(GL_LIGHTING);
					glPointSize(psize);
					glColor3f(0.f, 1.0f, 0.f);
					glBegin(GL_POINTS);
					for (int i = 0; i < frame->getParticleCount(); i++) {

						glm::vec3 pos = frame->getParticles()[i];
						float speed = frame->getSpeeds()[i];

						glm::vec3 custom;
						getPreviewColor(speed, 10.0f, primary, secondary, custom);


						glColor3f(custom[0], custom[1], custom[2]);
						glVertex3f(pos.x, pos.y, pos.z);

					}
					glEnd();
					glEnable(GL_LIGHTING);
				}
			}

		}
	}


	void ParticleCache::load(std::ifstream &) {

	}


	void ParticleCache::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> ParticleCache::exportData() {
		return mapEntityProperties(this);
	}

	bool ParticleCache::isExportable() {
		return true;
	}

	void *ParticleCache::creator(DfxGlobalEntityName uniquename) {
		return new ParticleCache(uniquename);
	}

	void ParticleCache::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(inType, "type", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		DfxEntityDB::createProperty(inDirPath, "cache_directory", new DfxEntityPropertyTypes::DirPathPropertyType());
		DfxEntityDB::createProperty(inPrefix, "prefix", new DfxEntityPropertyTypes::StringPropertyType());
		DfxEntityDB::createProperty(inParticleSize, "particle_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIndex, "index", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inPositionArray, "position", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(inVelocityArray , "velocity", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	void ParticleCache::cacheToMemory() {
		using namespace DfxEntityPropertyTypes;

		int frame_num = *(float*)getProperty(inIndex);

		Float3Array *_positions = (Float3Array*)getProperty(inPositionArray);
		Float3Array *_velocities = (Float3Array*)getProperty(inVelocityArray);

		if (!_positions->isNull() && _positions->size() > 0) {

			DfxGlobal::info::message("Caching Particles...");

			if (memCache.getFrame(frame_num) == nullptr) {

				SimulationFrame *frame = new SimulationFrame();
				frame->initFrame(_positions->size());

				for (int inx = 0; inx < _positions->size(); inx++) {

					glm::vec3 pos((*_positions)[inx].x, (*_positions)[inx].y, (*_positions)[inx].z);
					glm::vec3 vel(0);

					if (inx < _velocities->size()) {
						vel = glm::vec3((*_velocities)[inx].x, (*_velocities)[inx].y, (*_velocities)[inx].z);
					}
	
				    frame->getParticles()[inx] = pos;
					frame->getSpeeds()[inx]	  = glm::length(vel);
					frame->getVelocities()[inx] = vel;
				}

				frame->setIsCached(true);
				memCache.insertFrame(frame_num, frame);
			}	
		}

		DfxGlobal::info::message("");
	}

	void ParticleCache::cacheToDisk() {

		using namespace DfxEntityPropertyTypes;

		int frame_num = *(float*)getProperty(inIndex);
		std::string dirpath = *(std::string*)getProperty(inDirPath);
		std::string prefix  = *(std::string*)getProperty(inPrefix);
		filesystem::path path(dirpath);
		Float3Array *_positions = (Float3Array*)getProperty(inPositionArray);
		Float3Array *_velocities = (Float3Array*)getProperty(inVelocityArray);

		
		if (_positions->size() > 0) {

			DfxGlobal::info::message("Caching to Disk...");

			Partio::ParticlesDataMutable* p = Partio::create();
			Partio::ParticleAttribute positionAttr = p->addAttribute("position", Partio::VECTOR, 3);
			Partio::ParticleAttribute velocityAttr = p->addAttribute("velocity", Partio::VECTOR, 3);
			Partio::ParticleAttribute speedAttr = p->addAttribute("speed", Partio::FLOAT, 1);

			for (int i = 0; i < _positions->size(); i++) {
				
				glm::vec3 ppos((*_positions)[i].x, (*_positions)[i].y, (*_positions)[i].z);
				glm::vec3 pvel(0);

				if (i < _velocities->size()) {
					pvel = glm::vec3((*_velocities)[i].x, (*_velocities)[i].y, (*_velocities)[i].z);
				}

				glm::vec3 _vel = pvel;
				glm::normalize(_vel);
				float speed = glm::length(_vel);

				int particle = p->addParticle();

				float* pos = p->dataWrite<float>(positionAttr, particle);
				float* vel = p->dataWrite<float>(velocityAttr, particle);
				float* spd = p->dataWrite<float>(speedAttr, particle);

				pos[0] = ppos.x;
				pos[1] = ppos.y;
				pos[2] = ppos.z;

				vel[0] = pvel.x;
				vel[1] = pvel.y;
				vel[2] = pvel.z;

				*spd = speed;

			}

			std::string directory = dirpath;
			std::string fname = prefix;
			std::string ftype = ".bgeo";
			std::stringstream fullpath;

			if (fname.length() == 0)
				fname = "no_name";

			if (ftype.length() == 0)
				ftype = ".bgeo";

			fullpath << directory << "/" << fname << ".@" << frame_num << ftype;

			filesystem::create_directories(path);

			Partio::write(fullpath.str().c_str(), *p);
			
			p->release();
			DfxGlobal::info::message("");
		}
	}
}