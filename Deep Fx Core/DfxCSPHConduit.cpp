/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxCSPHConduit.h"

namespace dfxcore {

	namespace sph {

		Conduit::Conduit() {
			_W = NULL;
			_gradW = NULL;
		}

		void Conduit::setW(float(*W)(glm::vec3 &r)) {
			_W = W;
		}

		void Conduit::setGradW(glm::vec3(*gradW)(glm::vec3 &r)) {
			_gradW = gradW;
		}

		float Conduit::W(glm::vec3 &r) {
			return _W(r);
		}

		glm::vec3  Conduit::gradW(glm::vec3 &r) {
			return _gradW(r);
		}
	}
}