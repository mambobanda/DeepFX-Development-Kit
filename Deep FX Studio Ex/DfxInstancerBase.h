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

#include <SgNode.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <FlAny.h>
#include <DfxBoundsMinMax.h>

class DfxInstancerBase {
private:
	static size_t ID_GEN;
	size_t _id;

public:

	enum { POINT_PROXY = 0, AABB_PROXY = 1, SHAPE_PROXY = 2 };

	DfxInstancerBase();
	virtual ~DfxInstancerBase();

	size_t id();

	virtual size_t instanceCount() = 0;

	virtual bool useTransformMatrix(size_t index)	 = 0;

	virtual glm::vec3 instancePosition(size_t index) = 0;
	virtual glm::vec3 instanceRotation(size_t index) = 0;
	virtual glm::vec3 instanceScale(size_t index)	 = 0;

	virtual bool isEnabled() = 0;
	virtual bool instanceHasRotation(size_t index)  = 0;
	virtual bool instanceHasScale(size_t index)		= 0;
	virtual bool instanceHasTransform(size_t index) = 0;
	virtual bool instanceHasShape(size_t index)		= 0;

	virtual unsigned int limit() = 0;

	virtual glm::mat4 instanceTransform(size_t index) = 0;
	virtual glm::mat4 finalTransform(size_t index) = 0;

	virtual bool hidden(size_t index) = 0;

	virtual unsigned int instanceProxyType() = 0;
	virtual deepsg::SgNode *instanceShape(size_t index) = 0;
	virtual DfxBoundsMinMax instanceShapeAABB(size_t index) = 0;

	virtual void begin() = 0;
	virtual void end()   = 0;

};