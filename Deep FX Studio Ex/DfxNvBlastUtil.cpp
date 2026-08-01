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

#include <cstdlib>
#include "DfxNvBlastUtil.h"

namespace dfxblast {

	SimpleRandomGenerator::SimpleRandomGenerator() {
		remember = false;
	};

	float SimpleRandomGenerator::getRandomValue(){
		float r = (float)rand();
		r = r / RAND_MAX;
		return r;
	}
	
	void SimpleRandomGenerator::seed(int32_t seed){
		srand(seed);
	}

	SimpleRandomGenerator::~SimpleRandomGenerator() {};
}