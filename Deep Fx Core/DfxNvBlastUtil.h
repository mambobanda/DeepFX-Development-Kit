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

#include <NvBlastExtAuthoringTypes.h>

namespace dfxblast {

	class SimpleRandomGenerator : public Nv::Blast::RandomGeneratorBase
	{
	public:
		SimpleRandomGenerator();

		virtual float getRandomValue();
		virtual void seed(int32_t seed);

		virtual ~SimpleRandomGenerator();

	private:
		bool remember;
	};
}