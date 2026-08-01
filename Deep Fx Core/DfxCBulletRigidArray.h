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

namespace dfxcore {

	namespace bullet {

		class DfxCBulletRigidArray;

		class RigidCompilation {
		public:
			size_t index;
			btCollisionShape *shape;
			DfxCBulletRigidArray *ref;

			glm::vec3 pos;
			glm::vec3 rot;
			glm::vec3 scl;

			glm::mat4 transform();
		};

		static DfxGlobalEntityName DfxCBulletRigidArray_Identifier = "2F2X252X_DFX_BULLET_RIGID_ARRAY";

		class DfxCBulletRigidArray : public DfxCSimulationEntity {
		public:
			static const DfxPropertyID outID							= 0;
			static const DfxPropertyID outTransforms					= 1;
			static const DfxPropertyID inProxyMeshArray					= 2;
			static const DfxPropertyID inShapeType						= 3;
			static const DfxPropertyID inMass							= 4;
			static const DfxPropertyID inMassArray						= 5;
			static const DfxPropertyID inFriction						= 6;
			static const DfxPropertyID inFrictionArray					= 7;
			static const DfxPropertyID inBounciness						= 8;
			static const DfxPropertyID inBouncinessArray				= 9;
			static const DfxPropertyID inLinearDampening				= 10;
			static const DfxPropertyID inLinearDampeningArray			= 11;
			static const DfxPropertyID inAngularDampening				= 12;
			static const DfxPropertyID inAngularDampeningArray			= 13;
			static const DfxPropertyID inInitialLinearVelocity			= 14;
			static const DfxPropertyID inInitialLinearVelocityArray		= 15;
			static const DfxPropertyID inInitialAngularVelocity			= 16;
			static const DfxPropertyID inInitialAngularVelocityArray	= 17;

		private:
			
			std::vector<RigidCompilation*> shapes;
			void buildShape(size_t inx, DfxEntityPropertyTypes::ProxyObject &po);
		public:
			
			DfxCBulletRigidArray(DfxGlobalEntityName uniquename);
			virtual ~DfxCBulletRigidArray();

			void compute() override;

			void onDataChanged(DfxEntityDataInstance *instance) override;

			void init() override;

			void clear() override;

			void done() override;

			void importData(std::vector<std::pair<std::string, std::string>> data) override;

			std::vector<std::pair<std::string, std::string>> exportData() override;

			bool isExportable() override;

			static void *creator(DfxGlobalEntityName uniquename);

			static void initialize();

			void defaultSettings();

			std::vector<RigidCompilation*> getAllShapes();

			void setTransform(size_t inx, DfxEntityPropertyTypes::Matrix4x4 inmat);

			DfxEntityPropertyTypes::Matrix4x4  getTransform(size_t inx);
		};
	}
}