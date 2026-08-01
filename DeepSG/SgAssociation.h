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

	class SgAssociation : public SgDescription {
	private:
		unsigned int _kind;
		std::string _surface;
		std::string _path;
	public:
		enum { NONE = 1000, SURFACE_MATERIAL = 1001, GROUP_ASSET = 1002, VARIANT_ASSET = 1003, ASSET_ASSET = 1004 };

		SgAssociation();
		virtual ~SgAssociation();

		void setKind(unsigned int);
		unsigned int kind();

		std::string path();
		void setPath(std::string pth);

		std::string surface();
		void setSurface(std::string pth);

		static std::string enumToString(unsigned int em);
		static unsigned int stringToEnum(std::string);
	};

	
}