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

#pragma once

#include "FlCommon.h"
#include "FlAny.h"

namespace fl {
	class DataObjectProperty {
	private:
		std::vector<Any*> _anyList;
		bool _hasBinding;

	public:

		DataObjectProperty(std::vector<Any*> anyList, bool hasBinding);

		bool hasBinding();

		Any *any(size_t index);

		size_t size();

		template<typename T>
		inline void setData(size_t index, T val) {
			_anyList[index]->setData<T>(val);
		}

		template<typename T>
		inline T &data(size_t index) {
			return _anyList[index]->data<T>();
		}

		int stamp(size_t index) {
			return _anyList[index]->stamp();
		}

		void stampValue(size_t index) {
			_anyList[index]->stampValue();
		}

		template<typename T>
		inline T *rawData(size_t index) {
			return _anyList[index]->rawData<T>();
		}
	};

}