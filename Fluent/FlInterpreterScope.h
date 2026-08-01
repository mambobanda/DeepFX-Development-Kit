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

#include <string>
#include <unordered_map>
#include <vector>
#include "FlAny.h"

namespace fl {

	class InterpreterScope {
	protected:
		std::unordered_map<std::string, Any*> output_map;
		Space *_space;
	protected:
		void clear();

		Any *findHardLink(Any *);
	public:
		InterpreterScope(Space *space);
		~InterpreterScope();

		void addOutput(std::string path, Any*);
		Any *findOutput(std::string path);

		std::vector<Any*> fetchInput(std::string path);
	};
}