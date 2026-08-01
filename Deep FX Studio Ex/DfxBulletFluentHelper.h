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
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DfxFluentSimplexObject.h"
#include "DfxBulletShapeBuilder.h"
#include "DfxBulletAgent.h"

class DfxBulletFluentHelper {
public:

	DfxBulletAgent *spawnRigidBodyAgent(btDiscreteDynamicsWorld *dynamicsWorld, DfxFluentSimplexObject *object, btTransform startTransform, glm::vec3 scale, unsigned int shapeType, 
		std::string set, size_t uid);

	DfxBulletAgent *spawnStaticBodyAgent(btDiscreteDynamicsWorld *dynamicsWorld, DfxFluentSimplexObject *object, btTransform startTransform, glm::vec3 scale, unsigned int shapeType,
		std::string set, size_t uid);

	/*DfxBulletAgent *spawnKinematicRigidBodyAgent(btDiscreteDynamicsWorld *dynamicsWorldDfxFluentSimplexObject *object, btTransform startTransform, unsigned int shapeType);
	DfxBulletAgent *spawnSoftBodyAgent(btDiscreteDynamicsWorld *dynamicsWorldDfxFluentSimplexObject *object, btTransform startTransform);*/

	btTransform bulletTransform(glm::vec3 pos, glm::quat quat);
};