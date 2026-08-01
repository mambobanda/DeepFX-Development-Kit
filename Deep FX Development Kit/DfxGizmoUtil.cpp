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

#include "DfxGizmoUtil.h"


glm::vec3 DfxGizmoUtil::yxPlaneRayIntersectionPoint(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrig) {

	float xplaneDir = 0;

	if (rayOrigin.z < 0)
		xplaneDir = 1;
	else
		xplaneDir = -1;

	glm::vec3 unitY = glm::vec3(planeOrig.x, planeOrig.y + 1, planeOrig.z) - planeOrig;
	glm::vec3 unitX = glm::vec3(planeOrig.x + xplaneDir, planeOrig.y, planeOrig.z) - planeOrig;
	glm::vec3 normal = glm::normalize(glm::cross(unitY, unitX));

	float intDis = 0;
	glm::intersectRayPlane(rayOrigin, glm::normalize(rayDirection), planeOrig, normal, intDis);

	return rayOrigin + intDis * rayDirection;
}

glm::vec3 DfxGizmoUtil::xzPlaneRayIntersectionPoint(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrig) {

	float xzplaneDir = 0;

	if (rayOrigin.y > 0)
		xzplaneDir = 1;
	else
		xzplaneDir = -1;

	glm::vec3 unitY = glm::vec3(planeOrig.x, planeOrig.y, planeOrig.z + 1) - planeOrig;
	glm::vec3 unitX = glm::vec3(planeOrig.x + xzplaneDir, planeOrig.y, planeOrig.z) - planeOrig;
	glm::vec3 normal = glm::normalize(glm::cross(unitY, unitX));

	float intDis = 0;
	glm::intersectRayPlane(rayOrigin, glm::normalize(rayDirection), planeOrig, normal, intDis);

	return rayOrigin + intDis * rayDirection;
}


glm::vec3 DfxGizmoUtil::rotate90Y(float x, float y, float z) {
	glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(.0f, 1.f, 0.f));
	return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
}

glm::vec3 DfxGizmoUtil::rotate90X(float x, float y, float z) {
	glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(.0f, .0f, 1.f));
	return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
}

glm::vec3 DfxGizmoUtil::rotate90Z(float x, float y, float z) {
	glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1.0f, .0f, .0f));
	return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
}