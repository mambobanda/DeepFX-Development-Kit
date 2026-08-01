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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include "FlCommon.h"
#include "FlLink.h"
#include "FlUtil.h"

namespace fl {
	class IterationType  {
	public:
		enum {
			NONE = 0,
			FOR_EACH = 1,
			PEEK = 2,
			MAP = 3,
			FILTER = 4,
			PARTITION = 5,
			REDUCE = 6,
		};
	};
}