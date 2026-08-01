/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <ospray/ospray_util.h>
#include <ospray/ospray_cpp.h>
#include <ospray/ospray_cpp/ext/rkcommon.h>
#include <rkcommon/math/AffineSpace.h>

class OSPRayObject {
public:

	std::unique_ptr<ospray::cpp::Geometry> mesh;
	std::unique_ptr<ospray::cpp::GeometricModel> model;
	std::unique_ptr<ospray::cpp::Group> group;
	std::unique_ptr<ospray::cpp::Instance> instance;

	OSPRayObject();
	virtual ~OSPRayObject();

};

class OSPRayVolume {
public:


	std::unique_ptr<ospray::cpp::Volume> volume;
	std::unique_ptr<ospray::cpp::VolumetricModel> model;
	std::unique_ptr<ospray::cpp::Group> group;
	std::unique_ptr<ospray::cpp::Instance> instance;

	OSPRayVolume();
	virtual ~OSPRayVolume();
};

class OSPRayLight {
public:
	std::unique_ptr<ospray::cpp::Light> light;

	OSPRayLight();
	virtual ~OSPRayLight();

};

rkcommon::math::affine3f glmMat4ToAffine3(glm::mat4 mat);