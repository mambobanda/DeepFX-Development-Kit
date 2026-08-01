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

#include "VH5IResource.h"
#include "VAttribute.h"
#include "VBuffer.h"

namespace xvault {

	namespace h5 {
		
		h5IResource::h5IResource() {
		}

		h5IResource::~h5IResource() {
		
		}

		std::vector<Attribute*> &h5IResource::attributes() {
			return _attributes;
		}

		std::vector<Buffer*> &h5IResource::buffers() {
			return _buffers;
		}

		void h5IResource::addAttribute(Attribute *attr) {
			_attributes.push_back(attr);
		}
		
		void h5IResource::addBuffer(Buffer *buff) {
			_buffers.push_back(buff);
		}

		Attribute *h5IResource::findAttribute(std::string name) {
			for (auto attr: _attributes) {
				if (name == attr->name()) {
					return attr;
				}
			}

			return nullptr;
		}

		Buffer *h5IResource::findBuffer(std::string name) {

			for (auto buff : _buffers) {
				if (name == buff->name()) {
					return buff;
				}
			}

			return nullptr;
		}

	}

}