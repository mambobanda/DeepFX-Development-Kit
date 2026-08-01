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

#include "VH5OResource.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		h5OResource::h5OResource() {
			
		}

		h5OResource::~h5OResource() {
			for (auto attr : _attributes) delete attr;
			for (auto buff : _buffers) delete buff;
		}

		int h5OResource::attributeCount() {
			return (int)_attributes.size();
		}
		
		Attribute *h5OResource::attribute(size_t inx) {
			return _attributes[inx];
		}

		int h5OResource::bufferCount() {
			return (int)_buffers.size();
		}

		Buffer *h5OResource::buffer(size_t inx) {
			return _buffers[inx];
		}

	}
}