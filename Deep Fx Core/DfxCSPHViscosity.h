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
#include "DfxCSPHAbstractForce.h"



namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName SPHViscosity_Identifier = "2F2X252X_DFX_SPH_VISCOSITY";

		class SPHViscosity : public DfxCSimulationEntity, public AbstractForce {
		public:
			static const DfxPropertyID outID				= 0;
			static const DfxPropertyID inRef				= 1;
			static const DfxPropertyID inViscosity			= 2;
			static const DfxPropertyID inBoundaryViscosity	= 3;
			

		public:

			SPHViscosity(DfxGlobalEntityName uniquename);
			virtual ~SPHViscosity();

			void addForce() override;

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			void init() override;

			void clear() override;

			void done() override;

			static void initialize();

			void setDefaults();
		};
	}
}