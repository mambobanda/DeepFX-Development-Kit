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

#include <FlAnyMap.h>

class DfxKeyFrameInterpolation {
public:

	static float interpolateFloat(int key, fl::AnyMap<int, float> *map);
	static int interpolateInt(int key, fl::AnyMap<int, int> *map);
	static std::string interpolateString(int key, fl::AnyMap<int, std::string> *map);

};