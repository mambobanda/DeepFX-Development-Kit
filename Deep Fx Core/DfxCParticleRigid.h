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
#include "DfxCParticleCollider.h"
#include "DfxCDefaultCollisionResponse.h"
#include "DfxCBulletRigidHandle.h"
#include "DfxCSimulationEntity.h"
#include "DfxSimulationCoreGlobal.h"
#include "DfxCPBDGrainParticle.h"
#include "DfxPBDGrainParticleType.h"


namespace dfxcore {

	static DfxGlobalEntityName ParticleRigidObject_Identifier = "2F2X252X_DFX_PARTICLE_RIGID_OBJECT";

	class ParticleRigidObject : public DfxCSimulationEntity {
	private:
		Dfx3dMeshData *mdataPtr;
		DfxBounds aabb;
		DfxCParticleCollider<DfxCDefaultCollisionResponse> collider;
	public:
		static const DfxPropertyID outID		    = 0;
		static const DfxPropertyID inRef			= 1;
		static const DfxPropertyID inFlipNormals	= 2;
		static const DfxPropertyID inParticleRadius	= 3;
		static const DfxPropertyID inFriction		= 4;
		static const DfxPropertyID inBounciness		= 5;
		static const DfxPropertyID inPosition		= 6;
		static const DfxPropertyID inRotation		= 7;
		static const DfxPropertyID inScale			= 8;

	public:

		ParticleRigidObject(DfxGlobalEntityName uniquename);
		virtual ~ParticleRigidObject();

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

		void defaultSettings();

		bool isValid();

		void calcBounds();

		bool inBounds(glm::vec3 pos);

		void resolveCollison(glm::vec3 currentPos, glm::vec3 lastPos, glm::vec3 currentVel, glm::vec3 &newPos, glm::vec3 &newVel);

		glm::mat4  getTransform();
	};
	
}