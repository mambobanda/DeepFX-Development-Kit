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

#include "DfxOpenGLGlobal.h"

DfxOpenGLGlobal *DfxOpenGLGlobal::instance = nullptr;

DfxOpenGLGlobal::DfxOpenGLGlobal() {

}

DfxOpenGLGlobal::~DfxOpenGLGlobal() {

}

DfxOpenGLGlobal *DfxOpenGLGlobal::getInstance() {

	if (instance == nullptr) {
		instance = new DfxOpenGLGlobal();
		instance->initializeOpenGLFunctions();
	}

	return instance;
}

void DfxOpenGLGlobal::initDefaultOpenGL() {

	float lightDirection[3] = { -0.57735f, -0.57735f, -0.57735f };
	float v[4];

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	v[0] = v[1] = v[2] = 1.f*0.3f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = 1.1*0.9f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	v[0] = -lightDirection[0]; v[1] = -lightDirection[1]; v[2] = -lightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, v);

	v[0] = v[1] = v[2] = 1.f*0.2f; v[3] = 1.0f;
	glLightfv(GL_LIGHT1, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = 1.5*0.4f; v[3] = 1.0f;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	v[0] = lightDirection[0]; v[1] = lightDirection[1]; v[2] = lightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT1, GL_POSITION, v);

	glEnable(GL_NORMALIZE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);

}

void DfxOpenGLGlobal::initOpenGLFunctions() {
	initDefaultOpenGL();
}