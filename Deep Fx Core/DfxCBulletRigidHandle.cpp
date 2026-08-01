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

#include <iostream>
#include "DfxCBulletCollisionActions.h"
#include "DfxCBulletRigidHandle.h"


namespace dfxcore {

	namespace bullet {
		
		DfxCBulletRigidHandle::DfxCBulletRigidHandle(): DfxCBulletHandle(BULLET_RIGID_OBJECT) {

			uidPtr = nullptr; 
			meshDataPtr = nullptr;
			shape = nullptr;
			body = nullptr;
			mass = 0;
			shapeType = DfxBulletShapeType::TRIANGLES;

			collisionListeners.push_back(new DfxCAwake_CollisionAction);
		}
		
		DfxCBulletRigidHandle::~DfxCBulletRigidHandle() {

		}

		void DfxCBulletRigidHandle::setUidPtr(DfxEntityPropertyTypes::UID *ui) {
			uidPtr = ui;
		}

		DfxEntityPropertyTypes::UID *DfxCBulletRigidHandle::getUidPtr() {
			return uidPtr;
		}

		void DfxCBulletRigidHandle::setMeshDataPtr(Dfx3dMeshData *mdata) {
			meshDataPtr = mdata;
		}

		void DfxCBulletRigidHandle::clearShape() {
			delete shape;
			shape = nullptr;
		}

		void DfxCBulletRigidHandle::clearBody() {
			delete body;
			body = nullptr;
		}

		void DfxCBulletRigidHandle::clearState() {
			getState()->reset();
		}

		void DfxCBulletRigidHandle::setShapeType(DfxBulletShapeType typ) {
			shapeType = typ;
		}

		DfxBulletShapeType DfxCBulletRigidHandle::getShapeType() {
			return shapeType;
		}

		void DfxCBulletRigidHandle::setInitialLinearVelocity(glm::vec3 vel) {
			initialLinearVelocity = vel;
		}

		glm::vec3 DfxCBulletRigidHandle::getInitialLinearVelocity() {
			return initialLinearVelocity;
		}

		void DfxCBulletRigidHandle::setInitialAngularVelocity(glm::vec3 vel) {
			initialAngularVelocity = vel;
		}

		glm::vec3 DfxCBulletRigidHandle::getInitialAngularVelocity() {
			return initialAngularVelocity;
		}

		btCollisionShape *DfxCBulletRigidHandle::buildTriangleShape() {

			if (meshDataPtr != nullptr) {

				if (meshDataPtr->getVertexCount() > 3 && ((int)(meshDataPtr->getIndexCount() / 3)) >= 1) {

					btTriangleMesh *mesh = new btTriangleMesh();

					for (int i = 0; i < meshDataPtr->getIndexCount() / 3; i++) {
						int counter = i * 3;
						btVector3 vert0(meshDataPtr->getVertex(meshDataPtr->getIndex(counter)).x,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter)).y,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter)).z);

						btVector3 vert1(meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 1)).x,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 1)).y,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 1)).z);

						btVector3 vert2(meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 2)).x,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 2)).y,
							meshDataPtr->getVertex(meshDataPtr->getIndex(counter + 2)).z);

						mesh->addTriangle(vert0, vert1, vert2);

					}

					btVector3 scl(localScale.x, localScale.y, localScale.z);

					btGImpactMeshShape *_shape = new btGImpactMeshShape(mesh);
					_shape->setLocalScaling(scl);
					_shape->setMargin(0.0);
					_shape->postUpdate();
					_shape->updateBound();

					return _shape;
				}
			}
			return nullptr;
		}

		btCollisionShape *DfxCBulletRigidHandle::buildConvexHullShape() {
			if (meshDataPtr != nullptr) {

				if (meshDataPtr->getVertexCount() > 3 && ((int)(meshDataPtr->getIndexCount() / 3)) >= 1) {

					btConvexHullShape *_shape = new btConvexHullShape();

					for (int i = 0; i < meshDataPtr->getVertexCount(); i++) {
						
						btVector3 vert(meshDataPtr->getVertex(i).x, meshDataPtr->getVertex(i).y, meshDataPtr->getVertex(i).z);
						_shape->addPoint(vert);
					}

					btVector3 scl(localScale.x, localScale.y, localScale.z);

					_shape->setLocalScaling(scl);
					_shape->optimizeConvexHull();
					_shape->initializePolyhedralFeatures();
					
					return _shape;
				}
			}
			return nullptr;
		}

		void DfxCBulletRigidHandle::setupShape() {

			clearShape();

			if (shapeType == DfxBulletShapeType::TRIANGLES) {
				shape = buildTriangleShape();
			}
			else if (shapeType == DfxBulletShapeType::CONVEX_HULL) {
				shape = buildConvexHullShape();
			}
		}

		void DfxCBulletRigidHandle::setupBody() {
			if (shape != nullptr) {

				btTransform startTransform;
				startTransform.setFromOpenGLMatrix(glm::value_ptr(worldTransform()));

				float _mass = mass;
				getState()->STATIC = (_mass == 0.f) ? true : false;

				btVector3 localInertia(0, 0, 0);
				
				if (!getState()->STATIC && !getState()->IS_SLEEPING)
					shape->calculateLocalInertia(mass, localInertia);
				else
					_mass = 0.f;

				btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(_mass, myMotionState, shape, localInertia);
				
				body = new btRigidBody(rbInfo);
				body->setLinearVelocity(btVector3(initialLinearVelocity.x, initialLinearVelocity.y, initialLinearVelocity.z));
				body->setAngularVelocity(btVector3(initialAngularVelocity.x, initialAngularVelocity.y, initialAngularVelocity.z));
				body->setUserPointer((DfxCBulletHandle*)this);
			}
		}

		glm::mat4 DfxCBulletRigidHandle::worldTransform() {

			glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z));

			trans *= glm::rotate(glm::mat4(), glm::radians(worldRotation.x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(worldRotation.y), glm::vec3(0.f, 1.f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(worldRotation.z), glm::vec3(0.f, .0f, 1.f));

			return trans;
		}

		bool DfxCBulletRigidHandle::hasValidShape() {
			return (shape != nullptr);
		}

		bool DfxCBulletRigidHandle::isSimulationReady() {
			return (hasValidShape() && body != nullptr);
		}

		Dfx3dMeshData *DfxCBulletRigidHandle::getMeshDataPtr() {
			return meshDataPtr;
		}

		void DfxCBulletRigidHandle::setInitialPosition(glm::vec3 pos) {
			worldPosition = pos;
		}

		glm::vec3 DfxCBulletRigidHandle::getInitialPosition() {
			return worldPosition;
		}

		void DfxCBulletRigidHandle::setInitialRotation(glm::vec3 rot) {
			worldRotation = rot;
		}

		glm::vec3 DfxCBulletRigidHandle::getInitialRotation() {
			return worldRotation;
		}

		void DfxCBulletRigidHandle::setInitialScale(glm::vec3 scl) {
			localScale = scl;
		}

		void DfxCBulletRigidHandle::setMass(float ms) {
			mass = ms;
		}

		float DfxCBulletRigidHandle::getMass() {
			return mass;
		}

		void DfxCBulletRigidHandle::setKinematic(bool k) {
			getState()->KINEMATIC = k;
		}

		btCollisionShape *DfxCBulletRigidHandle::getShape() {
			return shape;
		}

		btRigidBody *DfxCBulletRigidHandle::getBody() {
			return body;
		}

		bool DfxCBulletRigidHandle::isKinematic() {
			return getState()->KINEMATIC;
		}

		glm::mat4 DfxCBulletRigidHandle::getBulletTransformNoScale() {

			glm::mat4 mat;
		
			btTransform trans;

			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(trans);
			}
			else {
				trans = body->getWorldTransform();
			}

			trans.getOpenGLMatrix(glm::value_ptr(mat));

			return mat;
		}

		glm::mat4 DfxCBulletRigidHandle::getBulletTransform() {

			glm::mat4 mat;
			if (mass > 0) {
				btTransform trans;

				if (body && body->getMotionState()) {
					body->getMotionState()->getWorldTransform(trans);
				}
				else {
					trans = body->getWorldTransform();
				}

				trans.getOpenGLMatrix(glm::value_ptr(mat));

				mat *= glm::scale(glm::mat4(), glm::vec3(localScale.x, localScale.y, localScale.z));
			}
			else {

				mat = glm::translate(glm::mat4(),worldPosition);

				mat *= glm::rotate(glm::mat4(), glm::radians(worldRotation.x), glm::vec3(1.f, .0f, .0f));
				mat *= glm::rotate(glm::mat4(), glm::radians(worldRotation.y), glm::vec3(0.f, 1.f, .0f));
				mat *= glm::rotate(glm::mat4(), glm::radians(worldRotation.z), glm::vec3(0.f, .0f, 1.f));

				mat *= glm::scale(glm::mat4(), glm::vec3(localScale.x, localScale.y, localScale.z));

			}

			return mat;
		}

		glm::vec3 DfxCBulletRigidHandle::getInitialScale() {
			return localScale;
		}

		void DfxCBulletRigidHandle::collisionResponse(btDiscreteDynamicsWorld *dynamicsWorld, DfxCBulletCollisionPacket &cpacket) {
			for (auto listener : collisionListeners) {
				listener->response(dynamicsWorld, this, cpacket);
			}
		}
	}
}