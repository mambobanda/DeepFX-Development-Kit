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

#include "DfxCSPHRigid.h"

namespace dfxcore {

	namespace sph {

		SPHRigid::SPHRigid(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("SPH Point Rigid");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

		}

		SPHRigid::~SPHRigid() {

		}

		void SPHRigid::compute() {
			using namespace DfxEntityPropertyTypes;

			if (propertyHasBinding(inPoints)) {

				Float3 pos = *(Float3*)getProperty(inPosition);
				Float3 rot = *(Float3*)getProperty(inRotation);

				Float3Array *arr = (Float3Array*)getProperty(inPoints);

				glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, pos.z));

				transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
				transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
				transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

				if (!arr->isNull()) {

					if (arr->size() > 0) {

						for (int inx = 0; inx < arr->size(); inx++) {
							glm::vec3 restPos((*arr)[inx].x, (*arr)[inx].y, (*arr)[inx].z);
							particles[inx]->x = glm::vec3(transMat * glm::vec4( restPos, 1.0f));
						}
					}
				}
			}
			
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}

		size_t SPHRigid::particleCount() {
			return particles.size();
		}

		Particle *SPHRigid::getParticle(size_t index) {
			return particles[index];
		}

		void SPHRigid::preInit() {
			using namespace DfxEntityPropertyTypes;

			if (propertyHasBinding(inPoints)) {
				pingParents(false);	

				if (particles.size() > 0)
					for (auto p : particles) delete p;

				particles.clear();

				Float3Array *arr = (Float3Array*)getProperty(inPoints);

				if (!arr->isNull()) {

					if (arr->size() > 0) {

						for (int inx = 0; inx < arr->size(); inx++) {

							Particle *p = new Particle(BOUNDARY_PARTICLE);
							p->x = glm::vec3((*arr)[inx].x, (*arr)[inx].y, (*arr)[inx].z);
							p->setAlive(true);

							particles.push_back(p);
						}
					}
				}			
			}
		}

		void SPHRigid::init() {
			
		}

		void SPHRigid::clear() {

		}

		void SPHRigid::done() {
			for (auto p : particles) {
				delete p;
			}

			particles.clear();
		}

		void SPHRigid::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHRigid::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHRigid::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHRigid::isExportable() {
			return true;
		}

		void *SPHRigid::creator(DfxGlobalEntityName uniquename) {
			return new SPHRigid(uniquename);
		}

		void SPHRigid::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
			DfxEntityDB::createProperty(inPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType());
			DfxEntityDB::createProperty(inRotation, "rotation", new DfxEntityPropertyTypes::Float3PropertyType());
		}
	}
}