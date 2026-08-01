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

#include <string>
#include <glm/vec2.hpp>
#include "DfxGraphEditorCommon.h"

class DfxGraphEditorControlPoint {
public:
	
	std::string path;

	int interpolation;
	int frame;
	float floatValue;
	glm::vec2 t1;
	glm::vec2 t2;

	bool anchorSelected;
	bool t1Selected;
	bool t2Selected;

	float posx;
	float posy;

	float c1x;
	float c1y;

	float c2x;
	float c2y;

	float ax;
	float ay;
	float bx;
	float by;
	float cx;
	float cy;
};