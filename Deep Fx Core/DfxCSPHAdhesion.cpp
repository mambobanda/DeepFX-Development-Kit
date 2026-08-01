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

#include "DfxCSPHAdhesion.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace sph {

		SPHAdhesion::SPHAdhesion(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("SPH Adhesion");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inStrength, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			*(size_t*)getProperty(outID) = getInstanceID();

			setDefaults();
		}

		SPHAdhesion ::~SPHAdhesion() {

		}

		void SPHAdhesion::addForce() {
			using namespace DfxEntityPropertyTypes;

			float adhesion = *(float*)getProperty(inStrength);

			Global::getInstance()->reset(_h, Global::CUBIC_KERNEL);

			#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					
					glm::vec3 adhesionForce(0.0);
					AdhesionKernel adhesionK;
					adhesionK.initAdhesionKernel(_h);

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						if (particles[i]->getIndex() == np->getIndex())
							continue;

						if (np->getType() == BOUNDARY_PARTICLE ||
							np->getType() == MOVING_BOUNDARY_PARTICLE) {

							glm::vec3 xij = particles[i]->x - np->x;
							float xij_length = glm::length(xij);

							if (pow2(xij_length) > 1.0e-9) {
								xij = (1.0f / xij_length) * xij;
								adhesionForce -= adhesion * boundary_mass[np->getIndex()] * xij * adhesionK.adhesionW(xij_length);
							}
						}
					}

					forces[i] += adhesionForce;
				}
			}

		}

		void SPHAdhesion::compute() {

			cleanAllProperties();
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}


		void SPHAdhesion::init() {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(outID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}
		}

		void SPHAdhesion::clear() {

		}

		void SPHAdhesion::done() {

		}

		void SPHAdhesion::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHAdhesion::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHAdhesion::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHAdhesion::isExportable() {
			return true;
		}

		void *SPHAdhesion::creator(DfxGlobalEntityName uniquename) {
			return new SPHAdhesion(uniquename);
		}

		void SPHAdhesion::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inStrength, "in_strength", new DfxEntityPropertyTypes::FloatPropertyType());
		}

		void SPHAdhesion::setDefaults() {
			*(float*)getProperty(inStrength) = 0.05f;
		}
	}
}