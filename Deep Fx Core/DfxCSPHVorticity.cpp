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

#include "DfxCSPHVorticity.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace sph{

		SPHVorticity::SPHVorticity(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("SPH Vorticity");

			DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outID);
			uid->setRootID(getInstanceID());

			setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(inStrength, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outID, DfxEntityPropertyState::OUTPUT_STATE);

			*(size_t*)getProperty(outID) = getInstanceID();

			setDefaults();
		}

		SPHVorticity ::~SPHVorticity() {

		}

		void SPHVorticity::addForce() {
			using namespace DfxEntityPropertyTypes;

			float vorticity = *(float*)getProperty(inStrength);

			Global::getInstance()->reset(_h, Global::CUBIC_KERNEL);

#pragma omp parallel default(shared)
			{
				#pragma omp for schedule(static)
				for (int i = 0; i < nbParticles; i++) {
					if (!particles[i]->isAlive()) continue;

					if (particles[i]->getType() != FLUID_PARTICLE)
						continue;

					glm::vec3 vorticityForce(0.0f);

					for (int j = 0; j < particles[i]->nbCount; j++) {
						Particle *np = particles[particles[i]->nbIndices[j]];

						if (!np->isAlive()) continue;

						if (np->getType() != FLUID_PARTICLE)
							continue;

						glm::vec3 vij = np->v - particles[i]->v;
						glm::vec3 w = vij * Global::getInstance()->conduit().gradW(particles[i]->x - np->x);
						float _w = glm::length(w);
						glm::vec3 p = ((_mass * particles[i]->x) + (_mass * np->x)) / (_mass + _mass);
						glm::vec3 n = p - particles[i]->x;
						float _n = (glm::length(n));

						if (_n != 0 && _w != 0) {
							glm::vec3 N = n / _n;
							vorticityForce += vorticity * ((w / _w) * n);
						}
					}

					forces[i] += vorticityForce * _mass;
				}
			}
		}

		void SPHVorticity::compute() {

			cleanAllProperties();
			getDataInstance()->makeClean();
			makeChildrenDirty();
		}


		void SPHVorticity::init() {
			using namespace DfxEntityPropertyTypes;

			UID *uid = (UID*)getProperty(outID);
			uid->clearChildren();

			if (propertyHasBinding(inRef)) {
				uid->addChild((UID*)getProperty(inRef));
			}
		}

		void SPHVorticity::clear() {

		}

		void SPHVorticity::done() {

		}

		void SPHVorticity::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHVorticity::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHVorticity::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHVorticity::isExportable() {
			return true;
		}

		void *SPHVorticity::creator(DfxGlobalEntityName uniquename) {
			return new SPHVorticity(uniquename);
		}

		void SPHVorticity::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(outID, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(inStrength, "in_strength", new DfxEntityPropertyTypes::FloatPropertyType());
		}


		void SPHVorticity::setDefaults() {
			*(float*)getProperty(inStrength) = 0.5f;
		}
	}
}