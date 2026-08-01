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

#include <DfxUtility.h>
#include "DfxCSimulationEntity.h"
#include "DfxCSPHParticle.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCSPHComponents.h"
#include "DfxCCompactSearch.h"
#include "DfxCSPHGlobal.h"
#include "DfxCSPHLiquid.h"
#include "DfxCSPHRigid.h"
#include "DfxCSPHBulletRigid.h"
#include "DfxCSPHShapeEmitter.h"
#include "DfxCSPHAbstractForce.h"
#include "DfxCSPHViscosity.h"
#include "DfxCSPHVorticity.h"
#include "DfxCSPHAdhesion.h"
#include "DfxCParticleRigid.h"
#include <DfxUtility.h>



namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName DivergenceFreeSPHSolver_Identifier = "2F2X252X_DFX_DIVERGENCE_FREE_SPH_SOLVER";

		class DivergenceFreeSPHSolver : public DfxCSimulationEntity {
		private:

			Particle **particles;
			size_t   nbparticles;

			CompactSearch compactSearch;

			float timestep;
			float particleSize;
			int   substep;
			bool  diveregenceFree;
			float densityError;
			float divergenceError;
			float pressureRelaxation;
			float rigidCouplingStiffness;
			float velocityDampening;
			float viscosity;
			DfxEntityPropertyTypes::Float3 force;
			
			bool validparticleRadius;

			void loadProperties();

			void calc_density_advance(Particle **particles, int nbParticles, glm::vec3 *vi, float *rhoi, float *boundary_masses);

			void correct_density_error(Particle **particles, int nbParticles, glm::vec3 *vi, float *boundary_masses,
				float *rho_grad, float *alpha, float *rhoi, glm::vec3 *psiForce);

			void DivergenceFreeSPHSolver::calc_density_change(Particle **particles, int nbParticles, glm::vec3 *vi, float *rho_grad, float *bmass);

			void correct_divergence_error(Particle **particles, int nbParticles, glm::vec3 *vi, float *rho_grad, float *rhoi,
				float *alpha, float *bmass, glm::vec3 *psiForce);

			void updateBulletDynamics(std::vector<DfxCSPHBulletRigidHandle*> &handles, glm::vec3 *psiForces);
		private:
			
			// Buffers
			float *bmasses;
			float *falpha;
			float *fbeta;
			float *rhoi;
			float *rho_grad;
			glm::vec3 *lastX;
			glm::vec3 *vi;
			glm::vec3 *forces;
			glm::vec3 *psiForce;

			// Simulation variables
			float h;
			float invh;
			float rdensity;
			float pradius;
			float mass;

		public:
			
			static const DfxPropertyID outSolverID					= 0;
			static const DfxPropertyID outTrigger					= 1;
			static const DfxPropertyID inRef						= 2;	
			static const DfxPropertyID inTrigger					= 3;
			static const DfxPropertyID inParticleRadius				= 4;
			static const DfxPropertyID inTimestep					= 5;
			static const DfxPropertyID inSubstep					= 6;
			static const DfxPropertyID inDivergenceFree				= 7;
			static const DfxPropertyID inDensityError				= 8;
			static const DfxPropertyID inDivergenceError			= 9;
			static const DfxPropertyID inPressureRelaxation			= 10;
			static const DfxPropertyID inRigidCouplingStiffness		= 11;
			static const DfxPropertyID inVelocityDampening			= 12;
			static const DfxPropertyID inViscosity					= 13;
			static const DfxPropertyID inForce						= 14;

		public:

			DivergenceFreeSPHSolver(DfxGlobalEntityName uniquename);
			virtual ~DivergenceFreeSPHSolver();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void postInit() override;

			void clear() override;

			void done() override;

			void load(std::ifstream &) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			void defaultSettings();

			void syncSettings();

			void resetForces();

			void addUniformForce();

			static void initialize();

			static void *creator(DfxGlobalEntityName uniquename);

			Particle *getParticle(size_t inx);

			Particle **getParticles();

			glm::vec3 *getForceChannel();

			float *getBoundaryMassChannel();

			size_t getParticleCount();

			float getKernelSpacing();

			float getMass();

			std::vector<DfxCSimulationEntity*> findRigidObjects();

			std::vector<DfxCSimulationEntity*> findBulletRigidObjects();

			std::vector<DfxCSimulationEntity*> findLiquidObjects();

			std::vector<DfxCSimulationEntity*> findEmitters();

			std::vector<DfxCSimulationEntity*> findBulletSolvers();

			std::vector<DfxCSimulationEntity*> findMeshRigidObjects();

			void resolveMeshCollisions(Particle **particles, int nbParticles, glm::vec3 *lastPos);

			std::vector<AbstractForce*> findForces();

			void applyForces();
		};
	}
}