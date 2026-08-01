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

#include <glm/geometric.hpp>
#include <DfxScene.h>
#include "util.h"
#include "DfxCSimulationEntity.h"
#include "DfxCDivergenceFreeSPHSolver.h"

namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName SPHParticleCache_Identifier = "2F2X252X_DFX_SPH_PARTICLE_CACHE";

		class SimulationFrame;
		class SPHParticleCache;

		class SPHParticleCache : public DfxCSimulationEntity {
		private:
			std::unordered_map<std::string, SimulationFrame*> frames;
		public:
			static const DfxPropertyID inFrame			= 0;
			static const DfxPropertyID inType			= 1;
			static const DfxPropertyID inParticleSize	= 2;
			static const DfxPropertyID inSource			= 3;
		public:

			SPHParticleCache(DfxGlobalEntityName uniquename);
			virtual ~SPHParticleCache();

			void clearSimulationCache();

			int getFrameCount();

			void insertFrame(int frame, SimulationFrame *sframe);

			SimulationFrame* getFrame(int index);

			void init()  override;
			
			void clear() override;
			
			void done()  override;

			void compute() override;

			void reset() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void draw() override;

			DfxBounds bounds() override;

			bool selectable() override;

			void calculateBounds() override;

			void load(std::ifstream &) override;

			void cacheDFSPHNode();

			void cacheSPHLiquid();

			void cacheSPHRigid();

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

		};

		class SimulationFrame {
		private:
			glm::vec3 *particles;
			glm::vec3 *velocities;
			float *speeds;
			int particleCount;
			bool cached;
			bool frameHasVelocity;
			bool frameHasSpeed;
		public:
			SimulationFrame();

			void clear();
			void initFrame(int count);

			glm::vec3  *getParticles();
			glm::vec3  *getVelocities();
			float *getSpeeds();
			int getParticleCount();
			bool isCached();
			void setIsCached(bool);

			void setHasSpeed(bool);
			void setHasVelocity(bool);
			bool hasSpeed();
			bool hasVelocity();
		};
	}
}