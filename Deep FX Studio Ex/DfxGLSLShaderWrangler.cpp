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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxGLSLShaderWrangler.h"

DfxGLSLShaderWrangler *DfxGLSLShaderWrangler::_instance = nullptr;

DfxGLSLShaderWrangler::DfxGLSLShaderWrangler() {
	compiledLinked = false;

	// SPH Point Sprite Shader 
	auto sphPointSpriteShader = new GlSlProgram;
	sphPointSpriteShader->ready = false;
	sphPointSpriteShader->name = "sph_point_sprite_shader";
	sphPointSpriteShader->fragmentShaderScript = ":/glsl/sph_point_sprite_frag";
	sphPointSpriteShader->vertexShaderScript = ":/glsl/sph_point_sprite_vert";

	// Volume Raycaster Shader 
	auto volumeRaycasterShader = new GlSlProgram;
	volumeRaycasterShader->ready = false;
	volumeRaycasterShader->name = "volume_raycaster_shader";
	volumeRaycasterShader->fragmentShaderScript = ":/glsl/volume_raycaster_frag";
	volumeRaycasterShader->vertexShaderScript = ":/glsl/volume_raycaster_vert";

	_glslPrograms[sphPointSpriteShader->name] = sphPointSpriteShader;
	_glslPrograms[volumeRaycasterShader->name] = volumeRaycasterShader;
}

DfxGLSLShaderWrangler::~DfxGLSLShaderWrangler() {

}

DfxGLSLShaderWrangler *DfxGLSLShaderWrangler::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxGLSLShaderWrangler;
	}

	return _instance;
}

void DfxGLSLShaderWrangler::compileLink() {
	DfxGlobal::console::println("Compiling Shaders Once...");

	for (auto p : _glslPrograms) {

		if (!p.second->program.addShaderFromSourceFile(QOpenGLShader::Vertex, p.second->vertexShaderScript)) {
			DfxGlobal::console::println("Vertex Shader '" + p.second->name + "' compilation error.");
			DfxGlobal::console::println(p.second->program.log().toStdString() + "\n");
			continue;
		}

		if (!p.second->program.addShaderFromSourceFile(QOpenGLShader::Fragment, p.second->fragmentShaderScript)) {
			DfxGlobal::console::println("Fragment Shader '" + p.second->name + "' compilation error.");
			DfxGlobal::console::println(p.second->program.log().toStdString() + "\n");
			continue;
		}

		if (!p.second->program.link()) {
			DfxGlobal::console::println("Shader Program '" + p.second->name + "' linker error.");
			DfxGlobal::console::println(p.second->program.log().toStdString() + "\n");
		}
		else {
			p.second->ready = true;
		}

	}

	compiledLinked = true;
}

DfxGLSLShaderWrangler::GlSlProgram *DfxGLSLShaderWrangler::findGLSLProgram(std::string name) {

	for (auto p : _glslPrograms) {
		if (name == p.second->name) {
			return p.second;
		}
	}

	return nullptr;
}

bool DfxGLSLShaderWrangler::isCompiledLinked() {
	return compiledLinked;
}