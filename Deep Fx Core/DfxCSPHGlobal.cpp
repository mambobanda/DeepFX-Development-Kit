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

#include "DfxCSPHGlobal.h"


namespace dfxcore {

	namespace sph {
		
		Global *Global::instance = nullptr;

		Global::Global() {
		}

		Global *Global::getInstance() {

			if (instance == nullptr) {
				instance = new Global();
			}

			return instance;
		}

		Conduit &Global::conduit() {
			return _conduit;
		}

		void Global::reset(float h, int type) {

			if (type == CUBIC_KERNEL) {

				initCubicSplineKernel(h);

				_conduit.setW(cubicSplineW);
				_conduit.setGradW(cubicSplineGradW);
			}
			else if (type == POLY6_KERNEL) {

				initPoly6Kernel(h);

				_conduit.setW(poly6W);
				_conduit.setGradW(poly6GradW);
			}
		}
	}

}