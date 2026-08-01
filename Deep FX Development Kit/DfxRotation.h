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

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm\gtx\transform.hpp>

class DfxRotation {
private:
	unsigned int order;
	glm::vec3 euler;
public:
	enum {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5,
	};

public:
	DfxRotation(unsigned int rorder = 0);
	DfxRotation(glm::vec3 eulerAngles, unsigned int rorder = 0);
	DfxRotation(float x, float y, float z, unsigned int rorder = 0);
	~DfxRotation();

	void setOrder(unsigned int rorder);
	unsigned int getOrder();

	glm::mat4 rotationMatrix(float x, float y, float z);
	glm::mat4 rotationMatrix(glm::vec3);
	glm::mat4 rotationMatrix();

	void setEulerAngles(glm::vec3 eulerAngles);
	void setEulerAngles(float x , float y, float z);
	glm::vec3 getEulerAngles();

};