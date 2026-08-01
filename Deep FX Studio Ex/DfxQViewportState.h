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

#include <glm/mat4x4.hpp>

class DfxQViewportState {
public:
	float viewPhi;
	float viewTheta;
	float viewLamda;
	float viewDepth;
	float viewPanX;
	float viewPanY;
	size_t cameraID;
	glm::mat4 transform;
};