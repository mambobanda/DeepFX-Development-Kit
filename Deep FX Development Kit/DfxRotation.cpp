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

#include "DfxRotation.h"

DfxRotation::DfxRotation(unsigned int rorder): DfxRotation(0.f, 0.f, 0.f, rorder){
}

DfxRotation::DfxRotation(glm::vec3 eulerAngles, unsigned int rorder) : DfxRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z, rorder) {
}

DfxRotation::DfxRotation(float x, float y, float z, unsigned int rorder) {
	euler = glm::vec3(x, y, z);
	
	if (rorder > 5) {
		order = 0;
	}
	else {
		order = rorder;
	}
}

DfxRotation::~DfxRotation() {

}

void DfxRotation::setOrder(unsigned int rorder) {
	order = rorder;
}

unsigned int DfxRotation::getOrder() {
	return order;
}

glm::mat4 DfxRotation::rotationMatrix(float x, float y, float z) {
	switch (order)
	{
	case 0:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, .0f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, .0f, 1.f));

		return transMat;
	}
		break;
	case 1:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, .0f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, .0f, 1.f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.f, .0f));
		

		return transMat;
	}
		break;
	case 2:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.0f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, 0.f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, .0f, 1.f));

		return transMat;
	}
		break;
	case 3:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.0f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, 0.f, 1.0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, .0f, 0.f));

		return transMat;
	}
		break;
	case 4:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, .0f, 1.0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, 0.f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.0f, 0.f));

		return transMat;
	}
		break;
	case 5:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(z), glm::vec3(0.f, .0f, 1.0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(y), glm::vec3(0.f, 1.f, .0f));
		transMat	 *= glm::rotate(glm::mat4(1.f), glm::radians(x), glm::vec3(1.f, .0f, 0.f));

		return transMat;
	}
		break;
	default:
		return glm::mat4(1.f);
		break;
	}
}

glm::mat4 DfxRotation::rotationMatrix(glm::vec3 eulerAngles) {
	return rotationMatrix(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

glm::mat4 DfxRotation::rotationMatrix() {
	return rotationMatrix(euler.x, euler.y, euler.z);
}

void DfxRotation::setEulerAngles(glm::vec3 eulerAngles) {
	euler = eulerAngles;
}

void DfxRotation::setEulerAngles(float x, float y, float z) {
	euler = glm::vec3(x, y, z);
}

glm::vec3 DfxRotation::getEulerAngles() {
	return euler;
}