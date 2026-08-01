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

#include "DfxOpenglInfo.h"


bool DfxOpenglInfo::openglSupport = false;
bool DfxOpenglInfo::initialized = false;
int DfxOpenglInfo::_major = 0;
int DfxOpenglInfo::_minor = 0;
std::string DfxOpenglInfo::versionStr = "";
std::string DfxOpenglInfo::extensionsStr = "";

void DfxOpenglInfo::init() {

	if (!initialized) {
		
		QSurfaceFormat format;
		format.setVersion(2, 1);
		format.setProfile(QSurfaceFormat::CompatibilityProfile);

		// QSurfaceFormat::setDefaultFormat(format);

		QOffscreenSurface surf;
		surf.create();

		if (surf.supportsOpenGL()) {

			openglSupport = true;

			QOpenGLContext ctx;
			ctx.create();
			ctx.makeCurrent(&surf);

			QOpenGLFunctions glF(&ctx);

			versionStr = (const char*)glF.glGetString(GL_VERSION);
			extensionsStr = (const char*)glF.glGetString(GL_EXTENSIONS);
			
			glF.glGetIntegerv(GL_MAJOR_VERSION, &_major);
			glF.glGetIntegerv(GL_MINOR_VERSION, &_minor);
		}
		else {
			qDebug("Opengl Not Supported...");
		}

		initialized = true;
	}
}

int DfxOpenglInfo::glVersionMajor() {
	return _major;
}

int DfxOpenglInfo::glVersionMinor() {
	return _minor;
}

std::string DfxOpenglInfo::glVersionStr() {
	return versionStr;
}

std::string DfxOpenglInfo::glExtensionStr() {
	return extensionsStr;
}