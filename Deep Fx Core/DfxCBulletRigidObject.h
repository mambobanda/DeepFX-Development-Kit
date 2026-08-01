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
#include <btBulletDynamicsCommon.h>
#include "DfxCBulletRigidHandle.h"
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"



namespace dfxcore {

	namespace bullet {

		static DfxGlobalEntityName DfxCBulletRigidObject_Identifier = "2F2X252X_DFX_BULLET_RIGID_OBJECT";

		class DfxCBulletRigidObject : public DfxCSimulationEntity {
		private:
			std::vector<DfxCBulletRigidHandle*> rigidHandles;
		public:
			static const DfxPropertyID outID						= 0;
			static const DfxPropertyID outTransform					= 1;
			static const DfxPropertyID inRef						= 2;
			static const DfxPropertyID inShapeType					= 3;
			static const DfxPropertyID inInitialState				= 4;
			static const DfxPropertyID inMass						= 5;	
			static const DfxPropertyID inFriction					= 6;
			static const DfxPropertyID inBounciness					= 7;
			static const DfxPropertyID inLinearDampening			= 8;
			static const DfxPropertyID inAngularDampening			= 9;
			static const DfxPropertyID inInitialLinearVelocity		= 10;
			static const DfxPropertyID inInitialAngularVelocity		= 11;
			static const DfxPropertyID inPosition					= 12;
			static const DfxPropertyID inRotation					= 13;
			static const DfxPropertyID inScale						= 14;
		public:

			DfxCBulletRigidObject(DfxGlobalEntityName uniquename);
			virtual ~DfxCBulletRigidObject();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void preInit() override;

			void init() override;

			void clear() override;

			void done() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			void setTransform(DfxEntityPropertyTypes::Matrix4x4 &);

			glm::mat4 getAbsoluteTransform();

			glm::vec3 getScale();

			void defaultSettings();

			std::vector<DfxCBulletRigidHandle*> &getRigidHandles();

			void clearHandles();

			void createHandlesFromMeshData(Dfx3dMeshData *);

			void createHandlesFromProxyMeshData(DfxEntityPropertyTypes::ProxyObjectArray *);

		};

	}
}


