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
#include "DfxCPBDGrainSolver.h"
#include "DfxPBDGrainParticleType.h"
#include "DfxSimulationCoreGlobal.h"

namespace dfxcore {

	namespace pbd {

		static DfxGlobalEntityName PBDGrainMapChannel_Identifier = "2F2X252X_DFX_PBD_GRAIN_MAP_CHANNEL";

		class PBDGrainMapChannel : public DfxCSimulationEntity {
		private:
			void mapPBDGrainNode(DfxEntityPropertyTypes::UID *);
		public:
			static const DfxPropertyID inSourceID = 0;
			static const DfxPropertyID outPositions = 1;
			static const DfxPropertyID outVelocities = 2;

		public:

			PBDGrainMapChannel(DfxGlobalEntityName uniquename);
			virtual ~PBDGrainMapChannel();

			void compute() override;

			void init() override;

			void clear() override;

			void done() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();
		};

	}
}