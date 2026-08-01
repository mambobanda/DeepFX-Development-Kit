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
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include <glm/gtc/type_ptr.hpp>
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCBulletRigidObject.h"
#include "DfxCBulletRigidArray.h"
#include "DfxCBulletGlueConstraint.h"
#include "DfxCBulletPointToPointConstraint.h"
#include "DfxCBulletHingeConstraint.h"
#include "Util.h"

namespace dfxcore {

	namespace bullet {

		static DfxGlobalEntityName DfxCBulletSolver_Identifier = "2F2X252X_DFX_BULLET_SOLVER";

		class DfxCBulletSolver: public DfxCSimulationEntity {
		private:

			btDefaultCollisionConfiguration *collisionConfiguration;
			btCollisionDispatcher *dispatcher;
			btBroadphaseInterface *overlappingPairCache;
			btSequentialImpulseConstraintSolver *solver;
			btDiscreteDynamicsWorld *dynamicsWorld;

			std::vector<std::pair<btRigidBody*, DfxCBulletRigidObject*>> rigidObjects;
			std::vector<std::pair<btRigidBody*, RigidCompilation*>> rigidObjectArrays;
		
			bool physicsWorldInitialised;
		private:
			void initRigidObjects(std::vector<DfxCSimulationEntity*>);
			void initGlueConstraints(std::vector<DfxCSimulationEntity*>);
			void initNormalContraints(std::vector<DfxCSimulationEntity*>);
		public:
			static const DfxPropertyID outSolverID		= 0;
			static const DfxPropertyID outTrigger		= 1;
			static const DfxPropertyID inRef			= 2;
			static const DfxPropertyID inTrigger		= 3;
			static const DfxPropertyID inMasterSlave    = 4;
			static const DfxPropertyID inTimestep		= 5;
			static const DfxPropertyID inSubstep		= 6;
			static const DfxPropertyID inForce			= 7;
		public:
			DfxCBulletSolver(DfxGlobalEntityName uniquename);
			virtual ~DfxCBulletSolver();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void postInit() override;

			void clear() override;

			void done() override;

			void initialisePhysicsWorld();

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			void defaultSettings();

			void stepSolver(float timestep);

			bool isPhysicsWorldInitialised();

			bool isSlaveMode();

			void processCollisons();
		};
	}
}