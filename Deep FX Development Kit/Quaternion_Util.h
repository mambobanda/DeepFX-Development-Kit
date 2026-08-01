#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

glm::dquat RotationBetweenVectors(glm::dvec3 start, glm::dvec3 dest);

glm::dquat LookAt(glm::dvec3 direction, glm::dvec3 desiredUp);

glm::dquat RotateTowards(glm::dquat q1, glm::dquat q2, double maxAngle);