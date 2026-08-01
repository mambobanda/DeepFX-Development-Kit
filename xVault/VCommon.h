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

namespace xvault {

	class File;
	class iResource;
	class oResource;
	class Attribute;
	class Buffer;
	class Node;
	class Document;

	using WalkCallback = void(*)(std::string path, iResource *input, void *input_data);

	namespace type {
		enum
		{
			INT = 0,
			SHORT = 1,
			LONG = 2,
			LONG_LONG = 3,
			UNSIGNED_INT = 4,
			UNSIGNED_SHORT = 5,
			UNSIGNED_LONG = 6,
			UNSIGNED_LONG_LONG = 7,
			FLOAT = 8,
			DOUBLE = 9,
			CHAR = 10,
			STRING = 11,
			BOOL = 12,
			FLOAT_2 = 13,
			FLOAT_3 = 14,
			FLOAT_4 = 15,
			FLOAT_9 = 16,
			FLOAT_16 = 17,
		};
	}

	namespace io {
		enum
		{
			OPEN = 0,
			OPEN_READ_WRITE = 1,
			OPEN_READ_ONLY = 2,
		};
	}
}