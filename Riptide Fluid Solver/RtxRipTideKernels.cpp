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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "RtxRiptideKernels.h"

namespace rtx {

	// Cubic
	Real CubicKernel::m_radius;
	Real CubicKernel::m_k;
	Real CubicKernel::m_l;
	Real CubicKernel::m_W_zero;

	// Poly 6
	Real Poly6Kernel::m_radius;
	Real Poly6Kernel::m_k;
	Real Poly6Kernel::m_l;
	Real Poly6Kernel::m_m;
	Real Poly6Kernel::m_W_zero;
}