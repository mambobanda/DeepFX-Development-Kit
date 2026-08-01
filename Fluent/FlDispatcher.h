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
#include <vector>
#include "FlCommon.h"

namespace fl {

	class Dispatcher {
	private:
		static size_t ID_GEN;
		size_t _uid;
	public:

		Dispatcher();
		virtual ~Dispatcher();

		size_t id();

		void attachAny(Any *any);
		void attachBranch(Any *any, std::vector<std::string> exclude = std::vector<std::string>());

		virtual void valueChanged(Any *any) = 0;
	};
}