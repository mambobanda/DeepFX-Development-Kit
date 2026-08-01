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

#include "FlAnyHelpers.h"

namespace fl {

	std::unordered_map<size_t, AnyHelperCopyMethod>  AnyHelperMap::_copyMap;
	std::unordered_map<size_t, AnyHelperClearMethod> AnyHelperMap::_clearMap;

	void copy(Any *from, Any *to) {
		auto itr = AnyHelperMap::_copyMap.find(from->type());

		if (itr != AnyHelperMap::_copyMap.end()) {
			itr->second(from, to);
		}
	}

	void clear(Any *data) {
		if (!data->isNull()) {
			auto itr = AnyHelperMap::_clearMap.find(data->type());

			if (itr != AnyHelperMap::_clearMap.end()) {
				itr->second(data);
			}
		}
	}

	Any *duplicate(Any *original) {
		auto _any = new fl::Any;
		fl::copy(original, _any);
		return _any;
	}

	void move(Any *from, Any *to) {
		fl::clear(to);

		to->setVoidData(from->voidData());
		to->setType(from->type());
		to->setIsArray(from->isArray());

		from->nullify();
	}


}