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

#include <Dfx3dMeshData.h>
#include <glm/matrix.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <DfxEntityBasicPropertyTypes.h>
#include <BulletCollision\Gimpact\btGImpactShape.h>
#include <BulletCollision\CollisionShapes\btTriangleMesh.h>
#include <btBulletDynamicsCommon.h>
#include "DfxCBulletHandle.h"
#include "DfxCBulletHandleType.h"

namespace dfxcore {

	namespace bullet {

		enum class DfxBulletShapeType {TRIANGLES, CONVEX_HULL};

		class DfxCBulletRigidHandle: public DfxCBulletHandle {
		private:
			DfxEntityPropertyTypes::UID *uidPtr;
			Dfx3dMeshData *meshDataPtr;
			
			glm::vec3 worldPosition;
			glm::vec3 worldRotation;

			glm::vec3 initialLinearVelocity;
			glm::vec3 initialAngularVelocity;

			DfxBulletShapeType shapeType;

			btCollisionShape *shape;
			btRigidBody* body;

			glm::vec3 localScale;

			float mass;
		public:

			DfxCBulletRigidHandle();
			
			virtual ~DfxCBulletRigidHandle();

			virtual void collisionResponse(btDiscreteDynamicsWorld *dynamicsWorld, DfxCBulletCollisionPacket &cpacket) override;

			void setUidPtr(DfxEntityPropertyTypes::UID *);
			DfxEntityPropertyTypes::UID *getUidPtr();

			void setMeshDataPtr(Dfx3dMeshData *);
			Dfx3dMeshData *getMeshDataPtr();

			void setInitialPosition(glm::vec3);
			glm::vec3 getInitialPosition();

			void setInitialRotation(glm::vec3);
			glm::vec3 getInitialRotation();

			void setInitialLinearVelocity(glm::vec3);
			glm::vec3 getInitialLinearVelocity();

			void setInitialAngularVelocity(glm::vec3);
			glm::vec3 getInitialAngularVelocity();
			
			void setInitialScale(glm::vec3);
			glm::vec3 getInitialScale();

			void setMass(float);
			float getMass();

			void setKinematic(bool);
			bool isKinematic();

			glm::mat4 worldTransform();

			void clearShape();
			void clearBody();
			void clearState();

			void setupShape();
			void setupBody();

			void setShapeType(DfxBulletShapeType);
			DfxBulletShapeType getShapeType();

			btCollisionShape *buildTriangleShape();
			btCollisionShape *buildConvexHullShape();

			btCollisionShape *getShape();
			btRigidBody *getBody();

			glm::mat4 getBulletTransformNoScale();
			glm::mat4 getBulletTransform();

			bool hasValidShape();
			bool isSimulationReady();
		};
	}
}