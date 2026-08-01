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

#include "DfxCPBDGrainMapChannel.h"


namespace dfxcore {

	namespace pbd {

		PBDGrainMapChannel::PBDGrainMapChannel(DfxGlobalEntityName uniquename) : DfxCSimulationEntity(uniquename) {
			setLogicalName("Map Grain Channels");

			setPropertyState(inSourceID, DfxEntityPropertyState::INPUT_STATE);
			setPropertyState(outPositions, DfxEntityPropertyState::OUTPUT_STATE);
			setPropertyState(outVelocities, DfxEntityPropertyState::OUTPUT_STATE);
		}

		PBDGrainMapChannel::~PBDGrainMapChannel() {

		}

		void PBDGrainMapChannel::compute() {
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

				mapPBDGrainNode(uid);

				makePropertyBindingDirty(outPositions);
				makePropertyBindingDirty(outVelocities);
				makeChildrenDirty();
			}

			cleanAllProperties();
			getDataInstance()->makeClean();
		}

		void PBDGrainMapChannel::init() {

		}

		void PBDGrainMapChannel::clear() {

		}

		void PBDGrainMapChannel::done() {
			using namespace DfxEntityPropertyTypes;

			Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
			Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

			out_positions->deleteArray();
			out_velocities->deleteArray();
		}

		void PBDGrainMapChannel::onDataChanged(DfxEntityDataInstance *instance) {

		}

		void PBDGrainMapChannel::importData(std::vector<std::pair<std::string, std::string>> data) {

		}

		std::vector<std::pair<std::string, std::string>> PBDGrainMapChannel::exportData() {
			return mapEntityProperties(this);
		}

		bool PBDGrainMapChannel::isExportable() {
			return true;
		}

		void *PBDGrainMapChannel::creator(DfxGlobalEntityName uniquename) {
			return new PBDGrainMapChannel(uniquename);
		}

		void PBDGrainMapChannel::initialize() {
			DfxCSimulationEntity::initialize();

			DfxEntityDB::createProperty(inSourceID, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
			DfxEntityDB::createProperty(outPositions, "positions", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
			DfxEntityDB::createProperty(outVelocities, "velocities", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		}

		void PBDGrainMapChannel::mapPBDGrainNode(DfxEntityPropertyTypes::UID *uid) {
			using namespace DfxEntityPropertyTypes;

			auto solvers = dfxutil::traitUIDSearch(*uid, GrainSolver_Identifier);

			if (solvers.size() > 0) {

				size_t inst_id = *(size_t*)getProperty(inSourceID);

				GrainSolver *ent = (GrainSolver*)solvers[0];

				Float3Array *out_positions = (Float3Array*)getProperty(outPositions);
				Float3Array *out_velocities = (Float3Array*)getProperty(outVelocities);

				if (ent != nullptr) {

					if (ent->getParticleCount() > 0) {
						std::vector<Particle*> tempArray;

						out_positions->deleteArray();
						out_velocities->deleteArray();

						for (int inx = 0; inx < ent->getParticleCount(); inx++) {
							if (ent->getParticle(inx)->isAlive() && ent->getParticle(inx)->getType() == GRANULAR_PARTICLE) {
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

	}
}