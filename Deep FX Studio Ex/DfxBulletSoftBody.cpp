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

#include "DfxBulletSoftBody.h"
#include "DfxBulletActorNode.h"

DfxBulletSoftBody::DfxBulletSoftBody(btDynamicsWorld *dWorld, btSoftBodyWorldInfo *sBodyWorldInfo): DfxBulletBodyBase(DfxBulletActorNode::Type::SOFT_BODY_OBJECT) {
	_dynamicsWorld = dWorld;
	softBodyWorldInfo = sBodyWorldInfo;
}

DfxBulletSoftBody::~DfxBulletSoftBody() {

}

btSoftBody *DfxBulletSoftBody::body() {
	return _body;
}

btDynamicsWorld *DfxBulletSoftBody::dynamicsWorld() {
	return _dynamicsWorld;
}

size_t DfxBulletSoftBody::nodeCount() {
	if (_body != nullptr) {
		return _body->m_nodes.size();
	}

	return 0;
}

glm::vec3 DfxBulletSoftBody::nodePosition(size_t index) {

	if (_body != nullptr) {
		auto pos = _body->m_nodes[index].m_x;
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}

	return glm::vec3(0);
}

glm::vec3 DfxBulletSoftBody::nodeVelocity(size_t index) {
	if (_body != nullptr) {
		auto vel = _body->m_nodes[index].m_v;
		return glm::vec3(vel.x(), vel.y(), vel.z());
	}

	return glm::vec3(0);
}

bool DfxBulletSoftBody::initBody(deepsg::SgNode *node) {

	if (node != nullptr) {

		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {
			
			auto geoNode = (deepsg::SgGeoNode*)node;
			auto adapter = geoNode->getGeometryAdapter();
			
			if ((adapter->getTriangleIndexListSize() / 3) > 2) {

				_body = btSoftBodyHelpers::CreateFromTriMesh(
					*softBodyWorldInfo,
					adapter->getFlatVertexList(),
					(int*)adapter->getTriangleIndexList(),
					adapter->getTriangleIndexListSize() / 3);

				return true;
			}
		}
	}

	return false;
}

void DfxBulletSoftBody::freeBody() {
	if (_dynamicsWorld != nullptr && _body != nullptr) {
		((btSoftRigidDynamicsWorld*)_dynamicsWorld)->removeSoftBody(_body);
		delete _body;
		_body = nullptr;
	}
}

void DfxBulletSoftBody::insertIntoWorld() {
	((btSoftRigidDynamicsWorld*)_dynamicsWorld)->addSoftBody(_body);
}

btRigidBody *DfxBulletSoftBody::toRigidBody() {
	return nullptr;
}

btSoftBody  *DfxBulletSoftBody::toSoftBody() {
	return _body;
}