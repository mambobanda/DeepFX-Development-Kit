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

#include "DfxVersionNameGlobal.h"


namespace DfxVersionNameGlobal {

	int version() {
		return versioninfo::_version;
	}

	int major() {
		return versioninfo::_major;
	}

	int minor() {
		return versioninfo::_minor;
	}

	std::string versionString() {
		return std::to_string(versioninfo::_version) + "." + std::to_string(versioninfo::_major) + "." + std::to_string(versioninfo::_minor);
	}

	std::string name() {
		return appname::_name;
	}

	std::string versionNameString() {
		return appname::_name + " v" + versionString();
	}

}