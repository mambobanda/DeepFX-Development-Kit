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

#include <unordered_map>
#include <functional>
#include "FlCommon.h"
#include "FlAny.h"


namespace fl {
	
	using AnyHelperCopyMethod = std::function<void(Any*, Any *)>;
	using AnyHelperClearMethod = std::function<void(Any*)>;

	class AnyHelperMap {
	public:
		static std::unordered_map<size_t, AnyHelperCopyMethod>  _copyMap;
		static std::unordered_map<size_t, AnyHelperClearMethod> _clearMap;
	};

	void copy(Any *from, Any *to);
	Any *duplicate(Any *original);
	void move(Any *from, Any *to);
	void clear(Any *from);

}