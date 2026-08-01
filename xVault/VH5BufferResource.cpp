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

#include "VH5BufferResource.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		h5OBufferResource::h5OBufferResource(std::string nme, std::string type) {

			_name = nme;

			_contentAttribute = new h5Attribute;
			_contentAttribute->setName("__content__");
			_contentAttribute->setType(type::STRING);
			_contentAttribute->setValueString("buffer");

			_typeAttribute = new h5Attribute;
			_typeAttribute->setName("__type__");
			_typeAttribute->setType(type::STRING);
			_typeAttribute->setValueString(type);

			addAttribute(_contentAttribute);
			addAttribute(_typeAttribute);

		}

		h5OBufferResource::~h5OBufferResource() {
			
		}

		int h5OBufferResource::attributeCount() {
			return _attributes.size();
		}

		Attribute *h5OBufferResource::attribute(size_t index) {
			return _attributes[index];
		}

		int h5OBufferResource::bufferCount() {
			return 1;
		}

		Buffer *h5OBufferResource::buffer(size_t) {
			return _buffer;
		}

		void h5OBufferResource::setBuffer(Buffer *buff) {
			_buffer = buff;
		}

		void h5OBufferResource::finalize(){
			addBuffer(_buffer);
		}
	}
}