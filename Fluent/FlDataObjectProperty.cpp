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

#include "FlDataObjectProperty.h"

namespace fl{

	DataObjectProperty::DataObjectProperty(std::vector<Any*> anyList, bool hasBinding) {
		_anyList = anyList;
		_hasBinding = hasBinding;
	}

	bool DataObjectProperty::hasBinding() {
		return _hasBinding;
	}

	Any *DataObjectProperty::any(size_t index) {
		return _anyList[index];
	}

	size_t DataObjectProperty::size() {
		return _anyList.size();
	}
}