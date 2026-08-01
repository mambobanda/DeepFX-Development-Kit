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


class DfxResource;
class DfxResourcePtr;

namespace resource {
	
	using ReleaseCallback = void(*)(void *ptr);

	namespace type {
		enum  {
			NONE = 1, GEOMETRY = 2, IMAGE = 3,
		};
	};

	namespace format {
		enum {
			NONE = 1, DS_MESH = 2, RAW_IMAGE = 3,
		};
	}

	namespace mode {
		enum {
			NONE = 1, PROJECT = 2, FOLDER = 3, RESOURCE = 4,
		};
	}
}