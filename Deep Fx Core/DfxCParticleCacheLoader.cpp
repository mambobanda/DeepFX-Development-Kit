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

#include "DfxCParticleCacheLoader.h"


namespace dfxcore {

	ParticleCacheLoader::ParticleCacheLoader(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename, DFX_CUSTOM_ITEM) {
		setLogicalName("Particle Cache Loader");

		setPropertyState(inFilePath, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inParticleSize, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inIndex, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPositionArray, DfxEntityPropertyState::OUTPUT_STATE);
		setPropertyState(outVelocityArray, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inIndex) = 0.0f;
		*(float*)getProperty(inParticleSize) = 3.0f;

		current_index = -1;
		frame = nullptr;
	}

	ParticleCacheLoader::~ParticleCacheLoader() {
		if (frame != nullptr) {
			frame->clear();
			delete frame;
		}
	}

	void ParticleCacheLoader::init() {

	}

	void ParticleCacheLoader::clear() {

	}

	void ParticleCacheLoader::done() {

	}

	void ParticleCacheLoader::compute() {
		using namespace DfxEntityPropertyTypes;

		loadParticleCache();
		loadOuputArrays();

		cleanAllProperties();
		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void ParticleCacheLoader::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void ParticleCacheLoader::draw() {
		initializeOpenGLFunctions();

		pingParents();
		if (!getDataInstance()->isClean()) compute();

		if (fileCacheMap.size() > 0) {

			int index = (int)*(float*)getProperty(inIndex);
			float psize = *(float*)getProperty(inParticleSize);
			
			if (index != current_index) {

				if (frame != nullptr) {
					frame->clear();
					delete frame;
				}
			
				frame = getFileCacheFrame(index);
			}


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

	void ParticleCacheLoader::setFilepath(std::string path) {

		*(std::string*)getProperty(inFilePath) = path;
		getDataInstance()->makeDirty();
	}

	void ParticleCacheLoader::load(std::ifstream &) {
		
	}

	void ParticleCacheLoader::loadParticleCache() {
		
		fileCacheMap.clear();

		std::string cacheFilepath = *(std::string*)getProperty(inFilePath);

		if (cacheFilepath.length() > 0) {
			Partio::ParticlesDataMutable* p = Partio::read(cacheFilepath.c_str());

			if (p) {

				std::string dir = getFileDirName(cacheFilepath);
				std::vector<std::string> files = getDirectoryFilenames(dir);

				if (files.size() == 0)
					return;

				for (std::string pth : files) {

					int inc = getFileIncrement(pth);

					if (inc > -1) {

						std::stringstream key;
						key << "frame_" << inc;

						std::pair<std::string, std::string> pair(key.str(), dir + "\\" + pth);
						fileCacheMap.insert(pair);
					}
				}

				if (fileCacheMap.size() <= 0)
					return;
			}
			else {
				return;
			}

			p->release();
			return;
		}

		makePropertyBindingDirty(outPositionArray);
		makePropertyBindingDirty(outVelocityArray);
		
	}

	void ParticleCacheLoader::loadOuputArrays() {
		using namespace DfxEntityPropertyTypes;

		if (fileCacheMap.size() > 0) {

			int index = (int)*(float*)getProperty(inIndex);
			SimulationFrame *frame = getFileCacheFrame(index);

			if (frame != nullptr) {
				size_t count = frame->getParticleCount();

				((DfxEntityPropertyTypes::Float3Array*)getProperty(outPositionArray))->deleteArray();
				((DfxEntityPropertyTypes::Float3Array*)getProperty(outVelocityArray))->deleteArray();

				if (count > 0) {
					((DfxEntityPropertyTypes::Float3Array*)getProperty(outPositionArray))->resize(count);
					((DfxEntityPropertyTypes::Float3Array*)getProperty(outVelocityArray))->resize(count);

					for (size_t i = 0; i < count; i++) {
						Float3 pos;
						pos.x = frame->getParticles()[i].x;
						pos.y = frame->getParticles()[i].y;
						pos.z = frame->getParticles()[i].z;

						Float3 vel;
						vel.x = frame->getParticles()[i].x;
						vel.y = frame->getParticles()[i].y;
						vel.z = frame->getParticles()[i].z;

						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outPositionArray))[i].x = frame->getParticles()[i].x;
						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outPositionArray))[i].y = frame->getParticles()[i].y;
						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outPositionArray))[i].z = frame->getParticles()[i].z;

						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outVelocityArray))[i].x = frame->getVelocities()[i].x;
						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outVelocityArray))[i].y = frame->getVelocities()[i].y;
						(*(DfxEntityPropertyTypes::Float3Array*)getProperty(outVelocityArray))[i].z = frame->getVelocities()[i].z;
					}

					makePropertyBindingDirty(outPositionArray);
					makePropertyBindingDirty(outVelocityArray);
				}
			}

			if (frame != nullptr) {
				frame->clear();
				delete frame;
			}

		}
	}


	void ParticleCacheLoader::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> ParticleCacheLoader::exportData() {
		return mapEntityProperties(this);
	}

	bool ParticleCacheLoader::isExportable() {
		return true;
	}

	void *ParticleCacheLoader::creator(DfxGlobalEntityName uniquename) {
		return new ParticleCacheLoader(uniquename);
	}

	void ParticleCacheLoader::initialize() {
		DfxCSimulationEntity::initialize();

		DfxEntityDB::createProperty(inFilePath, "cache_path", new DfxEntityPropertyTypes::FilePathPropertyType());
		DfxEntityDB::createProperty(inParticleSize, "particle_size", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inIndex, "index", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outPositionArray, "position", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(outVelocityArray, "velocity", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
	}

	SimulationFrame *ParticleCacheLoader::getFileCacheFrame(int frm) {

		if (fileCacheMap.size() > 0) {
			std::stringstream key;
			key << "frame_" << frm;

			std::unordered_map<std::string, std::string>::iterator it = fileCacheMap.find(key.str());

			if (it == fileCacheMap.end()) {
				return nullptr;
			}
			else {
				std::string path = it->second;

				Partio::ParticlesDataMutable* p = Partio::read(path.c_str());

				if (p) {
					Partio::ParticleAttribute positionAttr;
					Partio::ParticleAttribute speedAttr;
					Partio::ParticleAttribute velocityAttr;

					bool hasSpeedAttr = true;
					bool hasVelocityAttr = true;

					if (!p->attributeInfo("position", positionAttr))
						return nullptr;

					if (!p->attributeInfo("speed", speedAttr))
						hasSpeedAttr = false;

					if (!p->attributeInfo("velocity", velocityAttr))
						hasSpeedAttr = false;

					SimulationFrame *frame = new SimulationFrame;
					int pcount = p->numParticles();

					frame->initFrame(pcount);

					for (int i = 0; i < pcount; i++) {
						const float* pos = p->data<float>(positionAttr, i);

						frame->getParticles()[i].x = pos[0];
						frame->getParticles()[i].y = pos[1];
						frame->getParticles()[i].z = pos[2];

						if (hasSpeedAttr) {
							const float* pos = p->data<float>(speedAttr, i);
							frame->getSpeeds()[i] = *pos;

							if (!frame->hasSpeed()) frame->setHasSpeed(true);
						}
						else {
							frame->getSpeeds()[i] = 1.0f;
						}

						if (hasVelocityAttr) {
							const float* pos = p->data<float>(velocityAttr, i);
							frame->getVelocities()[i].x = pos[0];
							frame->getVelocities()[i].y = pos[1];
							frame->getVelocities()[i].z = pos[2];

							if (!frame->hasVelocity())  frame->setHasVelocity(true);
						}
						else {
							frame->getVelocities()[i] = glm::vec3(0.0f);
						}
					}

					p->release();

					frame->setIsCached(true);
					return frame;

				}
				else {
					return nullptr;
				}

				return nullptr;
			}
		}
		else {
			return nullptr;
		}
	}
}