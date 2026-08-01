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

#include "DfxCSPHLiquid.h"

namespace dfxcore {

	namespace sph {

		SPHLiquid::SPHLiquid(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("SPH Liquid");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inPoints, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			*(size_t*)getProperty(outID) = getInstanceID();
		}

		SPHLiquid::~SPHLiquid() {

		}

		void SPHLiquid::compute() {

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

								Particle *p = new Particle(FLUID_PARTICLE);
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

		size_t SPHLiquid::particleCount() {
			return particles.size();
		}

		Particle *SPHLiquid::getParticle(size_t index) {
			return particles[index];
		}

		void SPHLiquid::init() {

		}

		void SPHLiquid::clear() {
		
		}

		void SPHLiquid::done() {
			
			getDataInstance()->getPropertyInstance(inPoints)->dirtyParent();
			getDataInstance()->makeDirty();
		}

		void SPHLiquid::load(std::ifstream &) {
			*(size_t*)getProperty(outID) = getInstanceID();
		}

		void SPHLiquid::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHLiquid::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHLiquid::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHLiquid::isExportable() {
			return true;
		}

		void *SPHLiquid::creator(DfxGlobalEntityName uniquename) {
			return new SPHLiquid(uniquename);
		}

		void SPHLiquid::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inPoints,"points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		}
	}
}