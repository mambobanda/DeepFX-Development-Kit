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

#include "DfxBulletKinematicRigidBody.h"
#include "DfxBulletActorNode.h"


DfxBulletKinematicRigidBody::DfxBulletKinematicRigidBody(btDynamicsWorld *dWorld) : DfxBulletBodyBase(DfxBulletActorNode::Type::KINEMATIC_OBJECT) {
	_dynamicsWorld = dWorld;
	_shape = nullptr;
	_body = nullptr;
	_motionState = nullptr;
}

DfxBulletKinematicRigidBody::~DfxBulletKinematicRigidBody() {

}

btRigidBody *DfxBulletKinematicRigidBody::body() {
	return _body;
}

btCollisionShape *DfxBulletKinematicRigidBody::shape() {
	return _shape;
}

void DfxBulletKinematicRigidBody::setShape(btCollisionShape *shp) {
	_shape = shp;
}

void DfxBulletKinematicRigidBody::freeBody() {

	if (_shape != nullptr) {
		delete _shape;
		_shape = nullptr;
	}

	if (_motionState != nullptr) {
		delete _motionState;
		_motionState = nullptr;
	}

	if (_dynamicsWorld != nullptr && _body != nullptr) {
		if (_body->isInWorld()) {
			_dynamicsWorld->removeRigidBody(_body);
			delete _body;
			_body = nullptr;
		}
	}

}

btDefaultMotionState *DfxBulletKinematicRigidBody::motionState() {
	return _motionState;
}

btDynamicsWorld *DfxBulletKinematicRigidBody::dynamicsWorld() {
	return _dynamicsWorld;
}

void DfxBulletKinematicRigidBody::initBody(btTransform startTransform) {
	if (_shape != nullptr) {

		_motionState = new btDefaultMotionState(startTransform);

		_body = new btRigidBody(0, _motionState, _shape);

	}
}

void DfxBulletKinematicRigidBody::insertIntoWorld() {
	_dynamicsWorld->addRigidBody(_body);
}

btRigidBody *DfxBulletKinematicRigidBody::toRigidBody() {
	return _body;
}

btSoftBody  *DfxBulletKinematicRigidBody::toSoftBody() {
	return nullptr;
}