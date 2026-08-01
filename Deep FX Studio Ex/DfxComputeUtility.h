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

#include "DfxFluentChannelMap.h"

namespace compute {

	class AttributeRef {
	public:
		void *_data;
		bool exists;
		bool created;
		bool sameAsRequestedType;
	};

	template<typename T>
	T *extractAttrRef(AttributeRef &ref) {
		return (T*)ref._data;
	}

	template<typename T>
	AttributeRef getAttribute(DfxFluentChannelMap *map, std::string name, size_t type, bool create = false) {
		auto attr = map->at<T>(name);

		if (attr == nullptr) {
			if (create) {
				AttributeRef ref;

				map->addAttribute<T>(name);
				ref._data = map->at<T>(name);
				ref.exists = true;
				ref.created = true;
				ref.sameAsRequestedType = true;
				return ref;
			}
			else {
				AttributeRef ref;
				ref._data = nullptr;
				ref.exists = false;
				ref.created = false;
				ref.sameAsRequestedType = false;
				return ref;
			}
		}
		else {

			AttributeRef ref;
			ref._data = attr;
			ref.exists = true;
			ref.created = false;
			ref.sameAsRequestedType = (type == map->attributeType(name));
			return ref;
		}
	}
}