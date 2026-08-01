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

#include <glm/mat4x4.hpp>
#include <vector>
#include <SgNode.h>



class DfxQRenderInstanceInfo {
public:
	deepsg::SgNode *node;
	std::vector<glm::mat4> xforms;
	bool hidden;
	// size_t size;
};