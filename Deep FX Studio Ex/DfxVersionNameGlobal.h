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

#include <string>

namespace DfxVersionNameGlobal {
	
	namespace versioninfo {
		static int _version = 1;
		static int _major   = 3;
		static int _minor   = 0;
	}

	namespace appname {
		static std::string _name = "Deep FX Studio";
	}

	int version();
    int major();
	int minor();

	std::string versionString();

	static std::string name();

	std::string versionNameString();
};