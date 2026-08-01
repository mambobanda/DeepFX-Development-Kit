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

#include "DfxCSPHMapChannel.h"
#include "DfxCDivergenceFreeSPHSolver.h"
#include "DfxCSPHLiquid.h"
#include "DfxCSPHShapeEmitter.h"

namespace dfxcore {

	namespace sph {

		SPHMapChannel::SPHMapChannel(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Map SPH Channels");

			setPropertyState(inSourceID, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outPositions, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(outVelocities, DfxEntityPropertyState::OUTPUT_STATE);
		}

		SPHMapChannel::~SPHMapChannel() {

		}

		void SPHMapChannel::compute() {
			using namespace DfxEntityPropertyTypes;
			/*if (propertyHasBinding(inSourceID)) {
				size_t inst_id = *(size_t*)getProperty(inSourceID);

				if (DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, DivergenceFreeSPHSolver_Identifier)) {

					mapDFSPHNode();
				}
				else if (DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, SPHLiquid_Identifier)) {

					mapSPHLiquid();
				}
				else if (DfxSimulationCoreGlobal::getInstance()
					->simulationEntityIsOfType(inst_id, SPHShapeEmitter_Identifier)) {

					mapSPHEmitter();
				}
				makePropertyBindingDirty(outPositions);
				makePropertyBindingDirty(outVelocities);
				makeChildrenDirty();
			}*/

			if (propertyHasBinding(inSourceID)) {

				UID *uid = (UID*)getProperty(inSourceID);

				mapDFSPHNode(uid);

				makePropertyBindingDirty(outPositions);
				makePropertyBindingDirty(outVelocities);
				makeChildrenDirty();
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
		}

		void SPHMapChannel::init() {

		}

		void SPHMapChannel::clear() {

		}

		void SPHMapChannel::done() {
			using namespace DfxEntityPropertyTypes;

			Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
			Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

			out_positions->deleteArray();
			out_velocities->deleteArray();
		}

		void SPHMapChannel::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void SPHMapChannel::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> SPHMapChannel::exportData() {
			return mapEntityProperties(this);
		}

		bool SPHMapChannel::isExportable() {
			return true;
		}

		void *SPHMapChannel::creator(DfxGlobalEntityName uniquename) {
			return new SPHMapChannel(uniquename);
		}

		void SPHMapChannel::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(inSourceID, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(outPositions, "positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
			DfxEntityDB::createProperty(outVelocities, "velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		}

		void SPHMapChannel::mapDFSPHNode(DfxEntityPropertyTypes::UID *uid) {
			using namespace DfxEntityPropertyTypes;

			auto solvers = dfxutil::traitUIDSearch(*uid, DivergenceFreeSPHSolver_Identifier);

			if (solvers.size() > 0) {

				size_t inst_id = *(size_t*)getProperty(inSourceID);

				DivergenceFreeSPHSolver *ent = (DivergenceFreeSPHSolver*)solvers[0];

				Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
				Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

				if (ent != nullptr) {

					if (ent->getParticleCount() > 0) {
						std::vector<Particle*> tempArray;

						out_positions->deleteArray();
						out_velocities->deleteArray();

						for (int inx = 0; inx < ent->getParticleCount(); inx++) {
							if (ent->getParticle(inx)->isAlive() && ent->getParticle(inx)->getType() == FLUID_PARTICLE) {
								tempArray.push_back(ent->getParticle(inx));
							}
						}

						if (tempArray.size() > 0) {
							out_positions->resize(tempArray.size());
							out_velocities->resize(tempArray.size());

							for (int inx = 0; inx < tempArray.size(); inx++) {

								(*out_positions)[inx].x = tempArray[inx]->x.x;
								(*out_positions)[inx].y = tempArray[inx]->x.y;
								(*out_positions)[inx].z = tempArray[inx]->x.z;

								(*out_velocities)[inx].x = tempArray[inx]->v.x;
								(*out_velocities)[inx].y = tempArray[inx]->v.y;
								(*out_velocities)[inx].z = tempArray[inx]->v.z;

							}
						}
					}
				}
			}
		}

		void SPHMapChannel::mapSPHLiquid() {
			using namespace DfxEntityPropertyTypes;

			size_t inst_id = *(size_t*)getProperty(inSourceID);

			SPHLiquid *ent = (SPHLiquid*)DfxSimulationCoreGlobal::getInstance()->findSimulationEntity(inst_id);

			Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
			Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

			if (ent != nullptr) {


				if (ent->particleCount() > 0) {
					out_positions->deleteArray();
					out_velocities->deleteArray();

					out_positions->resize(ent->particleCount());
					out_velocities->resize(ent->particleCount());

					for (int inx = 0; inx < ent->particleCount(); inx++) {
						(*out_positions)[inx].x = ent->getParticle(inx)->x.x;
						(*out_positions)[inx].y = ent->getParticle(inx)->x.y;
						(*out_positions)[inx].z = ent->getParticle(inx)->x.z;

						(*out_velocities)[inx].x = ent->getParticle(inx)->v.x;
						(*out_velocities)[inx].y = ent->getParticle(inx)->v.y;
						(*out_velocities)[inx].z = ent->getParticle(inx)->v.z;
					}
				}
			}
		}

		void SPHMapChannel::mapSPHEmitter() {
			using namespace DfxEntityPropertyTypes;

			size_t inst_id = *(size_t*)getProperty(inSourceID);

			SPHShapeEmitter *ent = (SPHShapeEmitter*)DfxSimulationCoreGlobal::getInstance()->findSimulationEntity(inst_id);

			Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
			Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

			if (ent != nullptr) {


				if (ent->particleCount() > 0) {
					out_positions->deleteArray();
					out_velocities->deleteArray();

					out_positions->resize(ent->particleCount());
					out_velocities->resize(ent->particleCount());

					for (int inx = 0; inx < ent->particleCount(); inx++) {
						(*out_positions)[inx].x = ent->getParticle(inx)->x.x;
						(*out_positions)[inx].y = ent->getParticle(inx)->x.y;
						(*out_positions)[inx].z = ent->getParticle(inx)->x.z;

						(*out_velocities)[inx].x = ent->getParticle(inx)->v.x;
						(*out_velocities)[inx].y = ent->getParticle(inx)->v.y;
						(*out_velocities)[inx].z = ent->getParticle(inx)->v.z;
					}
				}
			}
		}

	}
}