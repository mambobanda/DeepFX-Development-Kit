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
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

class DfxBulletBodyBase {
private:
	size_t _type;
public:

	DfxBulletBodyBase(size_t typ);
	virtual ~DfxBulletBodyBase();

	size_t type();

	virtual void freeBody() = 0;
	virtual btRigidBody *toRigidBody() = 0;
	virtual btSoftBody  *toSoftBody()  = 0;

};