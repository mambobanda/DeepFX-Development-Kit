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

#include <vector>
#include "VResource.h"
#include "VH5IResource.h"

namespace xvault {

	namespace h5 {

		class h5OResource : public oResource, public h5::h5IResource {
		public:

			h5OResource();
			virtual ~h5OResource();

			int attributeCount() override;
			Attribute *attribute(size_t) override;

			int bufferCount() override;
			Buffer *buffer(size_t) override;

		};
	}

}