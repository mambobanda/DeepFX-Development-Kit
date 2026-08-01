
#include "quaternion_util.h"


// Returns a quaternion such that q*start = dest
glm::dquat RotationBetweenVectors(glm::dvec3 start, glm::dvec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	double cosTheta = glm::dot(start, dest);
	glm::dvec3 rotationAxis;

	if (cosTheta < -1 + 0.001) {
		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		// since it's often what you want to do.
		rotationAxis = glm::cross(glm::dvec3(0.0, 0.0, 1.0), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::dvec3(1.0, 0.0, 0.0), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	double s = sqrt((1 + cosTheta) * 2);
	double invs = 1 / s;

	return glm::dquat(
		s * 0.5,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);


}



// Returns a quaternion that will make your object looking towards 'direction'.
// Similar to RotationBetweenVectors, but also controls the vertical orientation.
// This assumes that at rest, the object faces +Z.
// Beware, the first parameter is a direction, not the target point !
glm::dquat LookAt(glm::dvec3 direction, glm::dvec3 desiredUp) {

	if (length2(direction) < 0.0001)
		return glm::dquat();

	// Recompute desiredUp so that it's perpendicular to the direction
	// You can skip that part if you really want to force desiredUp
	glm::dvec3 right = cross(direction, desiredUp);
	desiredUp = glm::cross(right, direction);

	// Find the rotation between the front of the object (that we assume towards +Z,
	// but this depends on your model) and the desired direction
	glm::dquat rot1 = RotationBetweenVectors(glm::dvec3(0.0, 0.0f, 1.0), direction);
	// Because of the 1rst rotation, the up is probably completely screwed up. 
	// Find the rotation between the "up" of the rotated object, and the desired up
	glm::dvec3 newUp = rot1 * glm::dvec3(0.0, 1.0, 0.0);
	glm::dquat rot2 = RotationBetweenVectors(newUp, desiredUp);

	// Apply them
	return rot2 * rot1; // remember, in reverse order.
}



// Like SLERP, but forbids rotation greater than maxAngle (in radians)
// In conjunction to LookAt, can make your characters 
glm::dquat RotateTowards(glm::dquat q1, glm::dquat q2, double maxAngle) {

	if (maxAngle < 0.001) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	double cosTheta = glm::dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1 * -1.0;
		cosTheta *= -1.0;
	}

	double angle = acos(cosTheta);

	// If there is only a 2° difference, and we are allowed 5°,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	// This is just like slerp(), but with a custom t
	double t = maxAngle / angle;
	angle = maxAngle;

	glm::dquat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
	res = glm::normalize(res);
	return res;

}