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

#include "DfxCSPHConduit.h"
#include "DfxCCubicSpline.h"
#include "DfxCPoly6.h"

namespace dfxcore {

	namespace sph {

		class Global {
		private:
			static Global *instance;
			Conduit _conduit;

			Global();
		public:

			enum { CUBIC_KERNEL = 0xff, POLY6_KERNEL };

			static Global *getInstance();

			Conduit &conduit();

			void reset(float h, int type);
		};

	}
}
