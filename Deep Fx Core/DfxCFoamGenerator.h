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

#include <btBulletDynamicsCommon.h>
#include <random>
#include <limits>
#include <mutex>
#include <glm/geometric.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <DfxArray.h>
#include <Geometry\Ray.h>
#include <Geometry\LineSegment.h>
#include <Geometry\Triangle.h>
#include "DfxCSPHComponents.h"
#include "DfxFoamCompactSearch.h"
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCSPHParticle.h"
#include "DfxCCompactSearch.h"
#include "DfxFoamParticle.h"
#include "DfxCCubicSpline.h"
#include "DfxCParticleRigid.h"
#include "util.h"

namespace dfxcore {

		static DfxGlobalEntityName FoamGenerator_Identifier = "2F2X252X_DFX_FOAM_GENERATOR";

		class FoamGenerator : public DfxCSimulationEntity {
		private:
			FoamParticle *foamParticles;
			size_t nbFoamParticles;

		private:

			using FoamParticleArray = DfxArray<FoamParticle>;

			float dbg_minAirPotential;
			float dbg_minAirPotentialClamped;
			float dbg_maxAirPotential;
			float dbg_maxAirPotentialClamped;

			int dbg_index;

			int dbg_particle_count;
			int dbg_culled_particle_count;
			std::mutex dbgPCountLock;
			void increaseParticleCount(int);
			
			std::mutex dbgAirLock;
			void setAirDebug(float potential, float clamped);

			float dbg_minWavePotential;
			float dbg_minWavePotentialClamped;
			float dbg_maxWavePotential;
			float dbg_maxWavePotentialClamped;

			std::mutex dbgWaveLock;
			void setWaveDebug(float potential, float clamped);

			float dbg_minEnergyPotential;
			float dbg_minEnergyPotentialClamped;
			float dbg_maxEnergyPotential;
			float dbg_maxEnergyPotentialClamped;

			std::mutex dbgEnergyLock;
			void setEnergyDebug(float potential, float clamped);

			void printDebugInfo();
			void printKineticEnergyDebugInfo();

			float trappedAirKernel(glm::vec3 xij, float h);
			float clampPotential(float potential, float min, float max);
			void generateFoam(sph::Particle **particles, size_t nbparticles, FoamParticleArray &result);
			void advectFoam(sph::Particle **particles, size_t nbparticles);
			void cullFoam();
			void resolveCollisionsBruteForce();
			void resolveCollisionsBVH();
			void appendFoam(FoamParticleArray &result);
			void analyseKineticEnergy(sph::Particle **particles, size_t nbparticles);
			void generateDiffuseParticle(float rv, float xr, float x_theta, float xh, glm::vec3 pos, 
					glm::vec3 vel, float timestep, glm::vec3 &out_pos, glm::vec3 &out_vel);
			glm::vec3 transformPoint(glm::mat4 m, glm::vec3);
			float calcAirPotential(sph::Particle **particles, sph::Particle *p, float h);
			float calcWavePotential(sph::Particle **particles, sph::Particle *p, float mass, float h, glm::vec3 *normals);

			void foamGenerationMode();
			void kineticAnalysisMode();

		public:

			static const DfxPropertyID outID						= 0;
			static const DfxPropertyID inRef						= 1;
			static const DfxPropertyID inTrigger					= 2;
			static const DfxPropertyID inMode						= 3;
			static const DfxPropertyID inTimestep					= 4;
			static const DfxPropertyID inSubstep					= 5;
			static const DfxPropertyID inMass						= 6;
			static const DfxPropertyID inMinLife					= 7;
			static const DfxPropertyID inMaxLife					= 8;
			static const DfxPropertyID inCollisionMargin			= 9;
			static const DfxPropertyID inBuoyancy					= 10;
			static const DfxPropertyID inDrag						= 11;
			static const DfxPropertyID inMaxAirThreshold			= 12;
			static const DfxPropertyID inMinAirThreshold			= 13;
			static const DfxPropertyID inMaxWaveThreshold			= 14;
			static const DfxPropertyID inMinWaveThreshold			= 15;
			static const DfxPropertyID inMaxEnergyThreshold			= 16;
			static const DfxPropertyID inMinEnergyThreshold			= 17;
			static const DfxPropertyID inMaxWaveParticlesPerSec		= 18;
			static const DfxPropertyID inMaxAirParticlesPerSec		= 19;
			static const DfxPropertyID inParticleSize				= 20;
			static const DfxPropertyID inGravity					= 21;
			static const DfxPropertyID inPositions					= 22;
			static const DfxPropertyID inVelocities					= 23;
			static const DfxPropertyID outPositions					= 24;
			static const DfxPropertyID outVelocities				= 25;
		public:

			FoamGenerator(DfxGlobalEntityName uniquename);
			virtual ~FoamGenerator();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void clear() override;

			void done() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			void defaultSettings();

			std::vector<DfxCSimulationEntity*> findRigidObjects();
		};
}