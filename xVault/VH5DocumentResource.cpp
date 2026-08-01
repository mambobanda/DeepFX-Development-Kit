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

#include "VH5DocumentResource.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		h5ODocumentResource::h5ODocumentResource(std::string nme) {
			
			_name = nme;
			
			_buffer = new h5Buffer;
			_contentAttribute = new h5Attribute;
			_contentAttribute->setName("__content__");
			_contentAttribute->setType(type::STRING);
			_contentAttribute->setValueString("document");

			addAttribute(_contentAttribute);
			addBuffer(_buffer);
		}

		h5ODocumentResource::~h5ODocumentResource() {
			_buffer->clear();
		}

		int h5ODocumentResource::attributeCount() {
			return 1;
		}

		Attribute *h5ODocumentResource::attribute(size_t) {
			return _contentAttribute;
		}

		int h5ODocumentResource::bufferCount() {
			return 1;
		}

		Buffer *h5ODocumentResource::buffer(size_t) {
			return _buffer;
		}

		void h5ODocumentResource::setText(std::string str) {
			//printf(str.c_str());

			if (str.length() > 0) {

				_buffer->setBufferSize(str.length() + 1);
				_buffer->setType(type::CHAR);
				_buffer->setName(_name);

				char *cstr = new char[str.length() + 1];
				strcpy_s(cstr, str.length() + 1, str.c_str());

				_buffer->setBufferChar(cstr);

			}
		}

	}
}