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

#include "DfxBulletDeformingRigidBody.h"
#include "DfxBulletActorNode.h"


DfxBulletDeformingRigidBody::DfxBulletDeformingRigidBody(btDynamicsWorld *dWorld) : DfxBulletBodyBase(DfxBulletActorNode::Type::RIGID_OBJECT) {
	_dynamicsWorld = dWorld;
	_shape = nullptr;
	_body = nullptr;
	_motionState = nullptr;
}

DfxBulletDeformingRigidBody::~DfxBulletDeformingRigidBody() {

}

btRigidBody *DfxBulletDeformingRigidBody::body() {
	return _body;
}

btCollisionShape *DfxBulletDeformingRigidBody::shape() {
	return _shape;
}

void DfxBulletDeformingRigidBody::setShape(btCollisionShape *shp) {
	_shape = shp;
}

void DfxBulletDeformingRigidBody::freeBody() {

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

btDefaultMotionState *DfxBulletDeformingRigidBody::motionState() {
	return _motionState;
}

btDynamicsWorld *DfxBulletDeformingRigidBody::dynamicsWorld() {
	return _dynamicsWorld;
}

void DfxBulletDeformingRigidBody::initBody(deepsg::SgNode *node, btTransform startTransform) {
	
	if (node != nullptr) {

		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

			auto geoNode = (deepsg::SgGeoNode*)node;
			auto adapter = geoNode->getGeometryAdapter();

			if (adapter->getVertexCount() > 0 && adapter->getPolygonCount() > 0) {

				int vertStride = sizeof(btVector3);
				int indexStride = 3 * sizeof(int);

				int *_indices = (int*)adapter->getTriangleIndexList();
				_gVertices = new btVector3[adapter->getVertexCount()];

				for (int i = 0; i < adapter->getVertexCount(); i++) {
					auto v = adapter->getVertex(i);
					_gVertices[i].setValue(v.x, v.y, v.z);
				}

				auto index_array = new btTriangleIndexVertexArray(
					adapter->getTriangleIndexListSize() / 3,
					_indices, indexStride, 
					adapter->getVertexCount(), 
					(btScalar*)&_gVertices[0].x(), 
					vertStride);

				_shape = new btBvhTriangleMeshShape(index_array, false);
			}

			if (_shape != nullptr) {
				_motionState = new btDefaultMotionState(startTransform);

				_body = new btRigidBody(0, _motionState, _shape);
			}
		}
	}
	
}

void DfxBulletDeformingRigidBody::insertIntoWorld() {
	_dynamicsWorld->addRigidBody(_body);
}

void DfxBulletDeformingRigidBody::updateShape(deepsg::SgNode *node) {

	if (node != nullptr) {

		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

			auto geoNode = (deepsg::SgGeoNode*)node;
			auto adapter = geoNode->getGeometryAdapter();

			if (adapter->getVertexCount() > 0 && adapter->getPolygonCount() > 0) {

				btVector3 aabbMin(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
				btVector3 aabbMax(-BT_LARGE_FLOAT, -BT_LARGE_FLOAT, -BT_LARGE_FLOAT);

				for (int i = 0; i < adapter->getVertexCount(); i++) {
					auto v = adapter->getVertex(i);
					auto bv = btVector3(v.x, v.y, v.z);

					aabbMax.setMax(bv);
					aabbMin.setMin(bv);

					_gVertices[i].setValue(v.x, v.y, v.z);

				}

				((btBvhTriangleMeshShape*)_shape)->refitTree(aabbMin, aabbMax);
				_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(_body->getBroadphaseHandle(), _dynamicsWorld->getDispatcher());
			}
		}

	}
}

btRigidBody *DfxBulletDeformingRigidBody::toRigidBody() {
	return _body;
}

btSoftBody  *DfxBulletDeformingRigidBody::toSoftBody() {
	return nullptr;
}