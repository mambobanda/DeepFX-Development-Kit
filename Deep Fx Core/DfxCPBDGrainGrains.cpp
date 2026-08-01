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

#include "DfxCPBDGrainGrains.h"

namespace dfxcore {

	namespace pbd {

		PBDGrains::PBDGrains(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Grains");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			*(size_t*)getProperty(outID) = getInstanceID();
		}

		PBDGrains::~PBDGrains() {
			if (particles.size() > 0)
				for (auto p : particles) delete p;
		}

		void PBDGrains::compute() {

			if (propertyHasBinding(inPoints)) {

				if (isPropertyBindingDirty(inPoints)) {

					using namespace DfxEntityPropertyTypes;

					if (particles.size() > 0)
						for (auto p : particles) delete p;

					particles.clear();

					Float3Array *arr = (Float3Array*)getProperty(inPoints);

					if (!arr->isNull()) {

						if (arr->size() > 0) {

							for (int inx = 0; inx < arr->size(); inx++) {

								Particle *p = new Particle(GRANULAR_PARTICLE);
								p->x = glm::vec3((*arr)[inx].x, (*arr)[inx].y, (*arr)[inx].z);
								p->setAlive(true);

								particles.push_back(p);
							}
						}
					}

					makePropertyBindingDirty(outID);
				}
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}

		size_t PBDGrains::particleCount() {
			return particles.size();
		}

		Particle *PBDGrains::getParticle(size_t index) {
			return particles[index];
		}

		void PBDGrains::init() {

		}

		void PBDGrains::clear() {

		}

		void PBDGrains::done() {

			getDataInstance()->getPropertyInstance(inPoints)->dirtyParent();
			getDataInstance()->makeDirty();
		}

		void PBDGrains::load(std::ifstream &) {
			*(size_t*)getProperty(outID) = getInstanceID();
		}

		void PBDGrains::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void PBDGrains::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> PBDGrains::exportData() {
			return mapEntityProperties(this);
		}

		bool PBDGrains::isExportable() {
			return true;
		}

		void *PBDGrains::creator(DfxGlobalEntityName uniquename) {
			return new PBDGrains(uniquename);
		}

		void PBDGrains::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		}
	}
}