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

#include "DfxCParticleCollider_btDbvtCallback.h"

namespace dfxcore {

	ParticleCollider_btDvtCallback::ParticleCollider_btDvtCallback() {
		hasCollision = false;
		ccw = false;
	}

	void ParticleCollider_btDvtCallback::Process(const btDbvtNode *node) {

		if (node->isleaf()) {

			if (hasCollision) return;

			DfxCParticleTriangle *tri = (DfxCParticleTriangle*)node->data;

			math::vec p1(tri->a.x, tri->a.y, tri->a.z);
			math::vec p2(tri->b.x, tri->b.y, tri->b.z);
			math::vec p3(tri->c.x, tri->c.y, tri->c.z);

			math::Triangle triangle;

			if (!ccw) {
				triangle = math::Triangle(p1, p2, p3);
			}
			else {
				triangle = math::Triangle(p3, p2, p1);
			}

			math::vec a(currentPos.x, currentPos.y, currentPos.z);
			math::vec b(lastPos.x, lastPos.y, lastPos.z);

			LineSegment lg(b, a);

			math::float3 point;

			if (lg.Intersects(triangle, nullptr, &point)) {
				penetrationPoint = glm::vec3(point.x, point.y, point.z);
				hasCollision = true;

				math::vec _norm = triangle.NormalCW();
				normal = glm::vec3(_norm.x, _norm.y, _norm.z);
				
			}
			else {
				hasCollision = false;
			}
		}
	};
}