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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <SgNode.h>
#include <glm/mat4x4.hpp>
#include "DfxTime.h"

class DfxTransformOverride {
public:

	virtual glm::mat4 evaluate(DfxTime time, glm::mat4 globalTransform, deepsg::SgNode *node=nullptr) = 0;

};