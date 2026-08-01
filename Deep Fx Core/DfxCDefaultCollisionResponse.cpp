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

#include "DfxCDefaultCollisionResponse.h"


DfxCDefaultCollisionResponse::DfxCDefaultCollisionResponse() {

}

DfxCDefaultCollisionResponse::~DfxCDefaultCollisionResponse() {

}

void DfxCDefaultCollisionResponse::response(float restitutionCoefficient, float _frictionCoeffient, float radius, glm::vec3 penetrationPoint, glm::vec3 normal, glm::vec3 velocity, glm::vec3 &newPosition, glm::vec3 &newVelocity) {

	// Target point is the closest non-penetrating position from the
			// new position.
	glm::vec3 targetNormal = normal;
	glm::vec3 targetPoint = penetrationPoint + radius * targetNormal;
	glm::vec3 colliderVelAtTargetPoint = velocity;

	// Get new candidate relative velocity from the target point.
	glm::vec3 relativeVel = newVelocity - colliderVelAtTargetPoint;
	float normalDotRelativeVel = glm::dot(glm::normalize(targetNormal), glm::normalize(colliderVelAtTargetPoint));
	glm::vec3 relativeVelN = normalDotRelativeVel * targetNormal;
	glm::vec3 relativeVelT = relativeVel - relativeVelN;

	// Check if the velocity is facing opposite direction of the surface
	// normal
	if (glm::dot(glm::normalize(targetNormal), glm::normalize(colliderVelAtTargetPoint)) < 0.0f) {
		// Apply restitution coefficient to the surface normal component of
		// the velocity
		glm::vec3 deltaRelativeVelN = (-restitutionCoefficient - 1.0f) * relativeVelN;
		relativeVelN *= -restitutionCoefficient;

		// Apply friction to the tangential component of the velocity
		// From Bridson et al., Robust Treatment of Collisions, Contact and
		// Friction for Cloth Animation, 2002
		// http://graphics.stanford.edu/papers/cloth-sig02/cloth.pdf
		if (pow2(glm::length(relativeVelT)) > 0.0) {
			float frictionScale = std::max(
				1.0f - _frictionCoeffient * deltaRelativeVelN.length() /
				relativeVelT.length(),
				0.0f);
			relativeVelT *= frictionScale;
		}

		// Reassemble the components
		newVelocity = relativeVelN + relativeVelT + colliderVelAtTargetPoint;

		if (glm::dot(glm::normalize(newVelocity), glm::normalize(colliderVelAtTargetPoint)) == 1.0f) {
			newVelocity = glm::vec3(0);
		}
	}

	// Geometric fix
	newPosition = targetPoint;
}