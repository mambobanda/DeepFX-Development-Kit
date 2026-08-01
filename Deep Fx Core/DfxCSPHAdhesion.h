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

		static DfxGlobalEntityName SPHAdhesion_Identifier = "2F2X252X_DFX_SPH_ADHESION";

		class SPHAdhesion : public DfxCSimulationEntity, public AbstractForce {
		public:
			static const DfxPropertyID outID		= 0;
			static const DfxPropertyID inRef		= 1;
			static const DfxPropertyID inStrength	= 2;
			
		public:

			SPHAdhesion(DfxGlobalEntityName uniquename);
			virtual ~SPHAdhesion();

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

		//
		class AdhesionKernel {
		private:
			float adhesion_radius;
			float adhesion_half_radius;
			float adhesion_a;
			float adhesion_2radius;
		public:
			AdhesionKernel() {
				adhesion_radius = 0.0f;
				adhesion_a = 0.0f;
				adhesion_2radius = 0.0f;
			}

			inline void initAdhesionKernel(float _h) {
				this->adhesion_radius = _h;
				this->adhesion_half_radius = 0.5f * _h;
				this->adhesion_a = 0.007f / pow(_h, 3.25);
				this->adhesion_2radius = 2.0f*_h;
			}

			inline float adhesionW(float _r) {
				float res = 0.0;
				const float r2 = _r * _r;
				const float radius2 = adhesion_radius * adhesion_radius;

				if (r2 <= radius2) {
					if (_r > adhesion_half_radius)
						res = adhesion_a * pow(-4.0*r2 / adhesion_radius + 6.0*_r - adhesion_2radius, 0.25);
				}

				return res;
			}
		};
	}
}