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
#pragma once

#include <glm\glm.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\intersect.hpp>
#include <glm\gtx\transform.hpp>

#define gizPI 3.14159265359

class DfxGizmoUtil{
public:
	static glm::vec3 yxPlaneRayIntersectionPoint(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrig);
	static glm::vec3 xzPlaneRayIntersectionPoint(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrig);
	static glm::vec3 zPlaneRayIntersectionPoint(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrig);

	static glm::vec3 rotate90Y(float x, float y, float z);
	static glm::vec3 rotate90X(float x, float y, float z);
	static glm::vec3 rotate90Z(float x, float y, float z);
};