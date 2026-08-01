/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <DfxGlobal.h>
#include <QOpenGLShaderProgram>
#include <unordered_map>


class DfxGLSLShaderWrangler {
private:

	static DfxGLSLShaderWrangler *_instance;
	DfxGLSLShaderWrangler();

public:

	class GlSlProgram {
	public:
		QOpenGLShaderProgram program;
		std::string name;
		bool ready;
		QString vertexShaderScript;
		QString fragmentShaderScript;
	};

private:

	bool compiledLinked;
	std::unordered_map<std::string, GlSlProgram*> _glslPrograms;

public:

	~DfxGLSLShaderWrangler();
	
	static DfxGLSLShaderWrangler *getInstance();

	void compileLink();
	GlSlProgram *findGLSLProgram(std::string name);

	bool isCompiledLinked();
};