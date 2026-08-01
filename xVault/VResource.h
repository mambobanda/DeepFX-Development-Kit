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

#include "VCommon.h"

namespace xvault {


	class oResource {
	public:
		
		virtual int attributeCount() = 0;
		virtual Attribute *attribute(size_t) = 0;

		virtual int bufferCount() = 0;
		virtual Buffer *buffer(size_t) = 0;

	};

	class iResource {
	public:

		virtual void addAttribute(Attribute*) = 0;
		virtual void addBuffer(Buffer*) = 0;

		virtual Attribute *findAttribute(std::string) = 0;
		virtual Buffer *findBuffer(std::string) = 0;
	};

}