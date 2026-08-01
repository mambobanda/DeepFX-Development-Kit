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

#include "SgDescription.h"

namespace deepsg {

	class SgVariants : public SgDescription {
	private:
		unsigned int _kind;
	public:
		enum { NONE = 1000, GEOMETRY = 1002, MATERIAL = 1003, LIGHT = 1004, CAMERA = 1005 };

		SgVariants();
		virtual ~SgVariants();

		void setKind(unsigned int);
		unsigned int kind();

		static std::string enumToString(unsigned int em);
		static unsigned int stringToEnum(std::string);
	};

	
}