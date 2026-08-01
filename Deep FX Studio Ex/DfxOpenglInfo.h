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
#include <QOpenGLFunctions> 
#include <QSurfaceFormat>
#include <QOffScreenSurface>

class DfxOpenglInfo {
private:

	static bool initialized;
	static bool openglSupport;
	static int _major;
	static int _minor;
	static std::string versionStr;
	static std::string extensionsStr;

public:

	static void init();
	
	static int glVersionMajor();
	static int glVersionMinor();
	static std::string glVersionStr();
	static std::string glExtensionStr();
};