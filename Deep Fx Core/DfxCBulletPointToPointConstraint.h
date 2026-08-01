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

#include <DfxBounds.h>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <btBulletDynamicsCommon.h>
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "Util.h"

namespace dfxcore {

	namespace bullet {

		static DfxGlobalEntityName BulletPointToPointConstraint_Identifier = "2F2X252X_DFX_BULLET_POINT_TO_POINT_CONSTRAINT";

		class BulletPointToPointConstraint : public DfxCSimulationEntity {
		private:
			btTypedConstraint *constraint;
		public:
			static const DfxPropertyID outID		= 0;
			static const DfxPropertyID inRigid01	= 1;
			static const DfxPropertyID inRigid02	= 2;
			static const DfxPropertyID inPivot01	= 3;
			static const DfxPropertyID inPivot02	= 4;
			static const DfxPropertyID inBreakable	= 5;
			static const DfxPropertyID inLimit		= 6;

		public:

			BulletPointToPointConstraint(DfxGlobalEntityName uniquename);
			virtual ~BulletPointToPointConstraint();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void clear() override;

			void done() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			btTypedConstraint *getConstraint();

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

		};
	}
}