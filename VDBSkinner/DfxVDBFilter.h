/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include "ExportDef.h"

extern "C"
{
	enum {
		DFX_VDB_ERODE_FILTER=0x100,
		DFX_VDB_DILATE_FILTER,
		DFX_VDB_MEDIAN_FILTER,
		DFX_VDB_GAUSSIAN_FILTER,
	};

	struct VDBFilter {
		unsigned int type;
		unsigned int iterations;
		float value;
		float width;
	};
}