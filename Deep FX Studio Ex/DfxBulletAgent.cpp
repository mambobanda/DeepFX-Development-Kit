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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxBulletAgent.h"

DfxBulletAgent::DfxBulletAgent() {
	reset();
}

void DfxBulletAgent::reset() {
	ready = false;
	updateTransformCache = false;
	updatePointCache = false;
	kinematic = false;
	deforming = false;
	rigidBody = false;
	isStatic = false;
	pcloud = nullptr;
	pointID = 0;
	body = nullptr;
	cache = nullptr;
	transformOverride = nullptr;
	pointCloudCacheFilter = nullptr;
	pointCacheDeformer = nullptr; 
	fluentBody = nullptr;
	btTrimesh = nullptr;
	scale = glm::vec3(1.f);
}