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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <SgGeoNode.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include "DfxBulletBodyBase.h"


class DfxBulletSoftBody : public DfxBulletBodyBase {
private:
	btSoftBody *_body;
	btDynamicsWorld *_dynamicsWorld;
	btSoftBodyWorldInfo *softBodyWorldInfo;

public:

	DfxBulletSoftBody(btDynamicsWorld *dWorld, btSoftBodyWorldInfo *softBodyWorldInfo);
	virtual ~DfxBulletSoftBody();

	btSoftBody *body();

	btDynamicsWorld *dynamicsWorld();

	size_t nodeCount();

	glm::vec3 nodePosition(size_t index);

	glm::vec3 nodeVelocity(size_t index);

	bool initBody(deepsg::SgNode *node);

	void freeBody();

	void insertIntoWorld();

	btRigidBody *toRigidBody() override;
	btSoftBody  *toSoftBody() override;
};