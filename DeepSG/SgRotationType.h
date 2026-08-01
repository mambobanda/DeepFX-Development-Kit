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



#include <string>

namespace deepsg {

	static const int SgRotationTypeCount = 7;
	
	static const std::string SgRotationTypeStr[]{
		"XYZ",
		"XZY",
		"YXZ",
		"YZX",
		"ZXY",
		"ZYX",
		"QUATERNION"
	};

	enum {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5,
		QUATERNION = 6,
	};

}