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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxOSPRayCommon.h"

// Object
OSPRayObject::OSPRayObject() {
	
}

OSPRayObject::~OSPRayObject() {
	
}

// Volume
OSPRayVolume::OSPRayVolume() {

}

OSPRayVolume::~OSPRayVolume() {

}

// light
OSPRayLight::OSPRayLight() {
	light = nullptr;
}

OSPRayLight::~OSPRayLight() {

}

// Utilty functions
rkcommon::math::affine3f glmMat4ToAffine3(glm::mat4 mat) {
	glm::mat4 xform = glm::transpose(mat);
	rkcommon::math::affine3f xfm;
	xfm.l.vx = rkcommon::math::vec3f(xform[0][0], xform[1][0], xform[2][0]);
	xfm.l.vy = rkcommon::math::vec3f(xform[0][1], xform[1][1], xform[2][1]);
	xfm.l.vz = rkcommon::math::vec3f(xform[0][2], xform[1][2], xform[2][2]);
	xfm.p = rkcommon::math::vec3f(xform[0][3], xform[1][3], xform[2][3]);

	return xfm;
}