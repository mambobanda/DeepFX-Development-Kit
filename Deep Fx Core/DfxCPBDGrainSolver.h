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

#pragma once

#include "DfxCPBDGrainParticle.h"
#include "DfxCSimulationEntity.h"
#include "DfxCPBDGrainDistanceConstraint.h"
#include "DfxCPBDGrainFrictionConstraint.h"
#include "DfxCPBDGrainPenetrationConstraint.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCPBDGrainCompactSearch.h"
#include "DfxPBDGrainParticleType.h"
#include "DfxCPBDGrainGrains.h"
#include "DfxCParticleRigid.h"
#include "DfxCPBDGrainPointRigid.h"
#include "Util.h"

namespace dfxcore {

	namespace pbd {

		static DfxGlobalEntityName GrainSolver_Identifier = "2F2X252X_DFX_PBD_GRAINS_SOLVER";

		class GrainSolver : public DfxCSimulationEntity {
		private:
			Particle **particles;
			size_t   nbparticles;

			CompactSearch compactSearch;

			std::vector<size_t> *constraint_array;

			float mass;
			float imass;
		public:
			static const DfxPropertyID outRef				= 0;
			static const DfxPropertyID inRef				= 1;
			static const DfxPropertyID inTrigger			= 2;
			static const DfxPropertyID inTimestep			= 3;
			static const DfxPropertyID inSubsteps			= 4;
			static const DfxPropertyID inIterations			= 5;
			static const DfxPropertyID inParticleRadius		= 6;
			static const DfxPropertyID inKinematicFriction	= 7;
			static const DfxPropertyID inStaticFriction		= 8;
			static const DfxPropertyID inCStiffness			= 9;
			static const DfxPropertyID inVDampening			= 10;
			static const DfxPropertyID inGravity			= 11;

		public:
			GrainSolver(DfxGlobalEntityName uniquename);
			
			virtual ~GrainSolver();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void postInit() override;

			void clear() override;

			void done() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			size_t getParticleCount();

			Particle *getParticle(size_t inx);

			std::vector<DfxCSimulationEntity*> findGrainObjects();

			// Calculate Forces
			void calc_forces(Particle **particles, int nbParticles, glm::vec3 *forces);

			void init_rigid_constraints(Particle **particles, int nbParticles);

			// 
			void intergrate_velocity_postion(Particle **particles, int nbParticles, glm::vec3 *forces, glm::vec3 *lastX);

			// Init Arena
			void init(Particle **particles, int nbParticles);

			// Solver
			void solvePenetrationConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX);

			void solveDistanceConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX);

			void solveFrictionConstraints(Particle **particles, int nbParticles, glm::vec3 *lastX);

			void calcFinalPositions(Particle **particles, int nbParticles, glm::vec3 *lastX);

			void calcInterpenetrationDistances(Particle **particles, int nbParticles);

			std::vector<DfxCSimulationEntity*> findRigidObjects();

			std::vector<DfxCSimulationEntity*> findPointRigidObjects();

			void resolveCollisions(Particle **particles, int nbParticles, glm::vec3 *lastPos);
		};
	}
}
