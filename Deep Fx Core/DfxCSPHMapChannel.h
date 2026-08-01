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

namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName SPHMapChannel_Identifier = "2F2X252X_DFX_SPH_MAP_CHANNEL";

		class SPHMapChannel : public DfxCSimulationEntity {
		private:
			void mapDFSPHNode(DfxEntityPropertyTypes::UID *);
			void mapSPHLiquid();
			void mapSPHEmitter();
		public:
			static const DfxPropertyID inSourceID		= 0;
			static const DfxPropertyID outPositions		= 1;
			static const DfxPropertyID outVelocities	= 2;

		public:

			SPHMapChannel(DfxGlobalEntityName uniquename);
			virtual ~SPHMapChannel();

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