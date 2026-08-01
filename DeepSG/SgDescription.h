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

namespace deepsg {

	enum { NO_DESCRIPTION = 1000, ASSET_DESCRIPTION = 1001, ASSOCIATION_DESCRIPTION = 1002, VARIANTS_DESCRIPTION = 1003, GROUP_DESCRIPTION = 1004 };

	class SgDescription {
	private:
		static size_t ID_GEN;
		size_t _id;

		unsigned int _type;
	public:
		enum { ASSET = 1001, ASSOCIATION = 1001, VARIANTS = 1002, GROUP = 1003, };

		SgDescription(unsigned int typ);
		virtual ~SgDescription();

		size_t id();

		unsigned int type();
	};

}