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
#include "DfxCSPHParticle.h"
#include "DfxCSPHBulletRigidHandle.h"
#include "Util.h"

namespace dfxcore {

	namespace sph {

		static DfxGlobalEntityName SPHBulletRigid_Identifier = "2F2X252X_DFX_SPH_BULLET_RIGID";

		class SPHBulletRigid : public DfxCSimulationEntity {
		private:
			std::vector<DfxCSPHBulletRigidHandle*> rigidHandles;
		public:
			static const DfxPropertyID outRef			= 0;
			static const DfxPropertyID inRef			= 1;
			static const DfxPropertyID inShowParticles  = 2;
			static const DfxPropertyID inCouplingType	= 3;
			static const DfxPropertyID inVoxelSize		= 4;

		public:

			SPHBulletRigid(DfxGlobalEntityName uniquename);
			virtual ~SPHBulletRigid();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			std::vector<DfxCSPHBulletRigidHandle*> &getRigidHandles();

			void preInit() override;

			void init() override;

			void clear() override;

			void done() override;

			void draw() override;

			static void initialize();
		};
	}
}