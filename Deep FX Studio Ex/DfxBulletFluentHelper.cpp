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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxBulletFluentHelper.h"

DfxBulletAgent *DfxBulletFluentHelper::spawnRigidBodyAgent(btDiscreteDynamicsWorld *dynamicsWorld, DfxFluentSimplexObject *object, 
	btTransform startTransform, glm::vec3 scale, unsigned int shapeType, std::string set, size_t uid) {

	DfxBulletAgent *agent = nullptr;
	btCollisionShape *shape = nullptr;
	btTriangleMesh *triMesh = nullptr;

	auto adapter = object->getGeometryAdapter();

	if (adapter->getVertexCount() > 0) {

		if (shapeType == 0) {
			shape = DfxBulletShapeBuilder::convexHullShapeFromGeometry(adapter, scale);
		}
		else if (shapeType == 1) {
			auto p = DfxBulletShapeBuilder::gimpactTirangleShapeFromGeometry(adapter, scale);
			shape = p.first;
			triMesh = p.second;
		}		
	}
	
	// Add to dynamics world if valid
	if (shape != nullptr) {

		bool validTarget = false;

		agent = new DfxBulletAgent;
		agent->fluentBody = new DfxBulletFluentBody;
		agent->fluent = true;
		//agent->fluentSet = set;
		agent->fluentUID = uid;
		agent->rigidBody = true;
		agent->scale = scale;
		agent->btTrimesh = triMesh;

		agent->fluentBody->shape = shape;

		auto _motionState = new btDefaultMotionState(startTransform);
		agent->fluentBody->rigidBody = new btRigidBody(0, _motionState, shape);

		float mass = 10.f;
		btVector3 inertia(0.f, 0.f, 0.f);
		shape->calculateLocalInertia(mass, inertia);
		agent->fluentBody->rigidBody->setMassProps(mass, inertia);

		dynamicsWorld->addRigidBody(agent->fluentBody->rigidBody);
	
		agent->ready = true;
	}

	return agent;
}

DfxBulletAgent *DfxBulletFluentHelper::spawnStaticBodyAgent(btDiscreteDynamicsWorld *dynamicsWorld, DfxFluentSimplexObject *object, btTransform startTransform, glm::vec3 scale, unsigned int shapeType,
	std::string set, size_t uid) {

	DfxBulletAgent *agent = nullptr;
	btCollisionShape *shape = nullptr;
	btTriangleMesh *triMesh = nullptr;

	auto adapter = object->getGeometryAdapter();

	if (adapter->getVertexCount() > 0) {

		if (shapeType == 0) {
			shape = DfxBulletShapeBuilder::convexHullShapeFromGeometry(adapter, scale);
		}
		else if (shapeType == 1) {
			auto p = DfxBulletShapeBuilder::gimpactTirangleShapeFromGeometry(adapter, scale);
			shape = p.first;
			triMesh = p.second;
		}
	}

	// Add to dynamics world if valid
	if (shape != nullptr) {

		agent = new DfxBulletAgent;
		agent->fluentBody = new DfxBulletFluentBody;
		agent->fluent = true;
		//agent->fluentSet = set;
		agent->fluentUID = uid;
		agent->rigidBody = true;
		agent->isStatic = true;
		agent->btTrimesh = triMesh;

		agent->fluentBody->shape = shape;

		auto _motionState = new btDefaultMotionState(startTransform);
		agent->fluentBody->rigidBody = new btRigidBody(0, _motionState, shape);

		dynamicsWorld->addRigidBody(agent->fluentBody->rigidBody);

		agent->ready = true;
	}

	return agent;
}

//DfxBulletAgent *DfxBulletFluentHelper::spawnKinematicRigidBodyAgent(DfxFluentSimplexObject *object, btTransform startTransform, unsigned int shapeType) {
//
//}
//
//DfxBulletAgent *DfxBulletFluentHelper::spawnSoftBodyAgent(DfxFluentSimplexObject *object, btTransform startTransform) {
//
//}

btTransform DfxBulletFluentHelper::bulletTransform(glm::vec3 pos, glm::quat quat) {

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(quat.x, quat.y, quat.z, quat.w));

	startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	return startTransform;
}