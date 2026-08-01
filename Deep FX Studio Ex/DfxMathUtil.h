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

#include <EulerAngles.h>
#include <SgTransform.h>
#include <DfxRay.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>

namespace math {

	#define dfxPI 3.14159265359

	inline glm::vec3 rotate90Y(float x, float y, float z) {
		glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(.0f, 1.f, 0.f));
		return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
	}

	inline glm::vec3 rotate90X(float x, float y, float z) {
		glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(.0f, .0f, 1.f));
		return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
	}

	inline glm::vec3 rotate90Z(float x, float y, float z) {
		glm::mat4 rotMat = glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(1.0f, .0f, .0f));
		return glm::vec3(rotMat * glm::vec4(x, y, z, 1.f));
	}

	inline  glm::vec3 extractCameraPosition(const glm::mat4 &vmat) {
		glm::mat3 rotMat(vmat);
		glm::vec3 d(vmat[3]);

		return -d * rotMat;
	}

	inline DfxRay calcScreenRay(int x, int y, int width, int height, glm::mat4 viewMat, glm::mat4 projMat, glm::vec3 cameraPosition) {

		glm::vec3 start;
		glm::vec3 end;

		start = glm::unProject(glm::vec3(x, height - y, 0),
			viewMat, projMat, glm::vec4(0, 0, width, height));

		end = glm::unProject(glm::vec3(x, height - y, 1),
			viewMat, projMat, glm::vec4(0, 0, width, height));

		DfxRay ray;

		ray.setNull(false);
		ray.start = start;
		ray.end = end;
		ray.origin = cameraPosition;
		ray.direction = glm::normalize(end - start);

		return ray;
	}

	//inline deepsg::SgTransform decomposeMatrix(glm::mat) {

	//	//glm::mat4 transformation; // your transformation matrix.
	//	//glm::vec3 scale;
	//	//glm::quat rotation;
	//	//glm::vec3 translation;
	//	//glm::vec3 skew;
	//	//glm::vec4 perspective;
	//	//glm::decompose(transformation, scale, rotation, translation, skew, perspective);
	//}
}