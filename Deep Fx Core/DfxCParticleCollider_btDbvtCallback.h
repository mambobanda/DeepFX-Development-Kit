/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <btBulletDynamicsCommon.h>
#include <Geometry\Ray.h>
#include <Geometry\LineSegment.h>
#include <Geometry\Triangle.h>
#include <glm/vec3.hpp>
#include "DfxCParticleTriangle.h"

namespace dfxcore {

	class ParticleCollider_btDvtCallback : public btDbvt::ICollide {
	public:
		glm::vec3 penetrationPoint;
		glm::vec3 normal;
		glm::vec3 lastPos;
		glm::vec3 currentPos;
		bool hasCollision;
		bool ccw;

		ParticleCollider_btDvtCallback();

		void Process(const btDbvtNode *node);
	};
};