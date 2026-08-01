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

#include "DfxCSPHViscosity.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace sph {

		SPHViscosity::SPHViscosity (DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("SPH Viscosity");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inViscosity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inBoundaryViscosity, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			*(size_t*)getProperty(outID) = getInstanceID();

			setDefaults();
		}

		SPHViscosity::~SPHViscosity () {

		}

		void SPHViscosity::addForce() {
			using namespace DfxEntityPropertyTypes;

			float viscosity = *(float*)getProperty(inViscosity);
			float bviscosity = *(float*)getProperty(inBoundaryViscosity);

			Global::getInstance()->reset(_h, Global::CUBIC_KERNEL);

#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						glm::vec3 xixj = particles[i]->x - np->x;
						glm::vec3 gradw = Global::getInstance()->conduit().gradW(xixj);
						glm::vec3 vxvj = particles[i]->v - np->v;

						if (np->getType() == FLUID_PARTICLE) {
							glm::vec3 ai = 2.0f * viscosity * (_mass / np->density) * vxvj * (glm::dot(xixj, gradw)) / (pow2(glm::length(xixj) + 0.01f*_h2));
							forces[i] += ai;
						}
						else {
							glm::vec3 ai = 2.0f * bviscosity * (boundary_mass[np->getIndex()] / particles[i]->density) * vxvj * (glm::dot(xixj, gradw)) / (pow2(glm::length(xixj) + 0.01f*_h2));
							forces[i] += ai;
						}
					}
				}
			}
	
		}

		void SPHViscosity::compute() {
			
			cleanAllProperties();
			getDataInstance()->makeClean();	
			makeChildrenDirty();
		}


		void SPHViscosity::init() {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(outID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}
		}

		void SPHViscosity::clear() {

		}

		void SPHViscosity::done() {

		}

		void SPHViscosity::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHViscosity::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHViscosity::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHViscosity::isExportable() {
			return true;
		}

		void *SPHViscosity::creator(DfxGlobalEntityName uniquename) {
			return new SPHViscosity (uniquename);
		}

		void SPHViscosity::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inViscosity, "in_viscosity", new DfxEntityPropertyTypes::FloatPropertyType());
			DfxEntityDB::createProperty(inBoundaryViscosity,"in_boundary_viscosity", new DfxEntityPropertyTypes::FloatPropertyType());
		}


		void SPHViscosity::setDefaults() {
			*(float*)getProperty(inViscosity) = 0.05f;
			*(float*)getProperty(inBoundaryViscosity) = 0.f;
		}
	}
}