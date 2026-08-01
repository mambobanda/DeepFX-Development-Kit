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
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include <SgGeoNode.h>
#include "DfxBulletBodyBase.h"


class DfxBulletDeformingRigidBody : public DfxBulletBodyBase {
private:
	btCollisionShape *_shape;
	btRigidBody *_body;
	btDefaultMotionState* _motionState;
	btDynamicsWorld *_dynamicsWorld;

private:
	btVector3 *_gVertices;
public:
	DfxBulletDeformingRigidBody(btDynamicsWorld *dWorld);
	virtual ~DfxBulletDeformingRigidBody();

	btRigidBody *body();

	void updateShape(deepsg::SgNode *node);
	btCollisionShape *shape();
	void setShape(btCollisionShape *);

	btDefaultMotionState *motionState();
	btDynamicsWorld *dynamicsWorld();

	void initBody(deepsg::SgNode *node, btTransform startTransform);

	void freeBody();

	void insertIntoWorld();

	btRigidBody *toRigidBody() override;
	btSoftBody  *toSoftBody() override;
};