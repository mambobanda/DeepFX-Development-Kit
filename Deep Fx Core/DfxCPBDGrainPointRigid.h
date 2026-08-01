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

#include "DfxCSimulationEntity.h"
#include "DfxPBDGrainParticleType.h"
#include "DfxCPBDGrainParticle.h"

namespace dfxcore {

	namespace pbd {

		static DfxGlobalEntityName GrainPointRigid_Identifier = "2F2X252X_DFX_PBD_GRAIN_POINT_RIGID";

		class GrainPointRigid : public DfxCSimulationEntity {
		private:
			std::vector<Particle*> particles;
		public:
			static const DfxPropertyID outID = 0;
			static const DfxPropertyID inPoints = 1;
			static const DfxPropertyID inPosition = 2;
			static const DfxPropertyID inRotation = 3;

		public:

			GrainPointRigid(DfxGlobalEntityName uniquename);
			virtual ~GrainPointRigid();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			size_t particleCount();

			Particle *getParticle(size_t index);

			void preInit() override;

			void init() override;

			void clear() override;

			void done() override;

			static void initialize();
		};
	}
}