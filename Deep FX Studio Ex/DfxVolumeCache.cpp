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

#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <DfxGlobal.h>
#include "DfxVolumeCache.h"
#include "DfxGLSLShaderWrangler.h"


size_t DfxVolumeCache::ID_GEN = 100;

#define BUFFER_OFFSET(i) ((void*)(i))

DfxVolumeCache::DfxVolumeCache() {
	_id = ++ID_GEN;

	_ready  = false;
	_cached = false;
	_cache_active = false;

	_volumeData = nullptr;

	XDIM = 0;
	YDIM = 0;
	ZDIM = 0;
}

DfxVolumeCache::~DfxVolumeCache() {

}

size_t DfxVolumeCache::id() {
	return _id;
}

deepsg::SgGridAdapterInterface *DfxVolumeCache::volumeData() {
	return _volumeData;
}

void DfxVolumeCache::setVolumeData(deepsg::SgGridAdapterInterface *data) {
	_volumeData = data;
}

fl::Any *DfxVolumeCache::any() {
	return _any;
}

void DfxVolumeCache::setAny(fl::Any *ay) {
	_any = ay;
}

void DfxVolumeCache::applyLightingState(float voxPixSize, bool transparency ) {

	glEnable(GL_BLEND);
	//glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LESS, 1.0f);
	if(!transparency) glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(voxPixSize);
}

void DfxVolumeCache::undoLightingState() {

	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPointSize(1.f);
}

void DfxVolumeCache::drawVolume(glm::mat4 mvp, glm::vec3 camPos) {
	
	//float VOX_SIZE = 0.05;
	//glm::mat4 trs(1.f);
	//trs = glm::scale(trs, glm::vec3(VOX_SIZE * XDIM, VOX_SIZE * YDIM, VOX_SIZE * ZDIM));

	auto shader = DfxGLSLShaderWrangler::getInstance()->findGLSLProgram("volume_raycaster_shader");
	if (shader != nullptr) if (shader->ready) {
		shader->program.bind();

		auto _mvp = shader->program.uniformLocation("MVP");
		auto _volume = shader->program.uniformLocation("volume");
		auto _step_size = shader->program.uniformLocation("step_size");
		auto _camPos = shader->program.uniformLocation("camPos");

		shader->program.setUniformValue(_mvp, QMatrix4x4(&glm::transpose(mvp)[0][0]));
		shader->program.setUniformValue(_volume, 0);
		shader->program.setUniformValue(_step_size, 1.f / float(XDIM), 1.f / float(YDIM), 1.f / float(ZDIM));
		shader->program.setUniformValue(_camPos, QVector3D(camPos.x, camPos.y, camPos.z));
	}

	glBindTexture(GL_TEXTURE_3D, densityTextureID);
	glBindVertexArray(cubeVAOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesID);
	
	glEnable(GL_BLEND);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	glDisable(GL_BLEND);

	glUseProgram(0);

}

void DfxVolumeCache::drawVolume(float voxPixSize, bool transparency) {
	size_t count = _volumeData->channel(0)->voxelCount();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);


	glBindBuffer(GL_ARRAY_BUFFER, voxelCoordVBOID);
	glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, voxelIntensityVBOID);
	glColorPointer(4, GL_FLOAT, 4 * sizeof(float), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, voxelNormalVBOID);
	glNormalPointer(GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

	applyLightingState(voxPixSize, transparency);
	glDrawArrays(GL_POINTS, 0, count);
	undoLightingState();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

void DfxVolumeCache::clear() {
	if (_cache_active) {
		/*
		glDeleteVertexArrays(1, &cubeVAOID);
		glDeleteBuffers(1, &cubeVBOID);
		glDeleteBuffers(1, &cubeIndicesID);

		glDeleteTextures(1, &densityTextureID);
		*/
		//glDeleteVertexArrays(1, &voxelVAOID);
		glDeleteBuffers(1, &voxelCoordVBOID);
		glDeleteBuffers(1, &voxelIntensityVBOID);
		//glDeleteBuffers(1, &voxelNormalVBOID);

		_cache_active = false;
	}

	_ready = false;
	_cached = false;
}

bool DfxVolumeCache::isCached() {
	return _cached;
}

bool DfxVolumeCache::isReady() {
	return _ready;
}

int DfxVolumeCache::index3d(int i, int j, int k, int size_i, int size_j, int size_k) {
	size_t index = i + j * size_i + size_i * size_j * k;

	if (size_i == 0 || size_j == 0 || size_k == 0) throw std::invalid_argument("Unknown Dimension");

	if (index >= (size_i * size_j * size_k))  std::out_of_range("Base Array Out Of Range");

	return index;
}

bool DfxVolumeCache::cacheDensityTexture() {

	if (_volumeData == nullptr) 
		return false;

	if (_volumeData->channelCount() <= 0) 
		return false;

	if (_volumeData->channel(0)->valueType() != deepsg::SgGridChannel::FLOAT) 
		return false;


	int MAX_DIM = 0;

	auto dim = _volumeData->channel(0)->dimension();

	MAX_DIM = std::max(dim.x, std::max(dim.y, dim.z));
	
	XDIM = MAX_DIM;
	YDIM = MAX_DIM;
	ZDIM = MAX_DIM;

	int tRes = 500;
	// read the volume data file
	auto minMax = _volumeData->channel(0)->floatMinMax();

	float scale = 1.0 / (minMax.second - minMax.first);
	auto pData = _volumeData->channel(0)->flattenGridFloat((tRes*tRes*tRes), tRes, tRes, tRes, [=](float value) {
		return (value - minMax.first) * scale;
	});
	
	/*for (int xdim = 0; xdim < MAX_DIM; xdim++) {
		for (int ydim = 0; ydim < MAX_DIM; ydim++) {
			for (int zdim = 0; zdim < MAX_DIM; zdim++) {
				if (_volumeData->channel(0)->hasValue(xdim, ydim, zdim)) {

					auto inx = index3d(xdim, ydim, zdim, MAX_DIM, MAX_DIM, MAX_DIM);
					float value = _volumeData->channel(0)->floatValue(xdim, ydim, zdim);
					value = (value - minMax.first) * scale;
					pData[inx] = value;

				}
			}
		}
	}*/

	// generate OpenGL texture
	glGenTextures(1, &densityTextureID);
	glBindTexture(GL_TEXTURE_3D, densityTextureID);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// set the mipmap levels (base and max)
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);

	// allocate data with internal format and foramt as (GL_RED)	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, tRes, tRes, tRes, 0, GL_RED, GL_FLOAT, pData);
	// GL_CHECK_ERRORS

	// generate mipmaps
	glGenerateMipmap(GL_TEXTURE_3D);

	// delete the volume data allocated on heap
	delete[] pData;

	return true;
}
void DfxVolumeCache::cacheVolume() {
	initializeOpenGLFunctions();

	if (_volumeData != nullptr) {

		if (_volumeData->channelCount() > 0) {

			if (_volumeData->channel(0)->valueType() == deepsg::SgGridChannel::FLOAT) {

				std::vector<float> coords;
				std::vector<float> intensity;
				std::vector<float> gradient;
				std::vector<float> colors;

				auto minMax = _volumeData->channel(0)->floatMinMax();
				float scale_denom = (minMax.second - minMax.first);
				float scale = (scale_denom != 0) ? 1.0 / (minMax.second - minMax.first): 0.f;
				_volumeData->channel(0)->allCoordinatesGradientsAndFloatValues(1.0f, coords, intensity, gradient, [=](float value) {
					return (scale == 0) ? 1.f : (value - minMax.first) * scale;
				});

				if (coords.size() > 0) {

					colors.resize(intensity.size() * 4);

					for (long int inx = 0; inx < intensity.size(); inx++) {

						auto offset00 = inx * 4;
						auto offset01 = inx * 3;

						auto lightPos = glm::vec3( 20.57735f, 20.57735f, 20.57735f );
						auto voxelPos = glm::vec3(coords[offset01], coords[offset01 + 1], coords[offset01 + 2]);
						auto normal = glm::normalize(glm::vec3(gradient[offset01], gradient[offset01 + 1], gradient[offset01 + 2])) * -1.f;
						auto lightDir = glm::normalize(lightPos - voxelPos);
						auto diffuse = std::max(glm::dot(normal, lightDir), 0.f) * glm::vec3(1.f, 1.f, 1.f) * 2.2f;

						colors[offset00] = std::max(0.5f, diffuse.x);
						colors[offset00 + 1] = std::max(0.5f, diffuse.y);
						colors[offset00 + 2] = std::max(0.5f, diffuse.z);
						colors[offset00 + 3] = intensity[inx] / 2.f;
							
					}
					
					glGenBuffers(1, &voxelCoordVBOID);
					glGenBuffers(1, &voxelIntensityVBOID);
					// glGenBuffers(1, &voxelNormalVBOID);

					glEnableClientState(GL_VERTEX_ARRAY);
					glEnableClientState(GL_COLOR_ARRAY);
					// glEnableClientState(GL_NORMAL_ARRAY);

					
					glBindBuffer(GL_ARRAY_BUFFER, voxelCoordVBOID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coords.size(), &coords[0], GL_STATIC_DRAW);
					glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));		

					glBindBuffer(GL_ARRAY_BUFFER, voxelIntensityVBOID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), &colors[0], GL_STATIC_DRAW);
					glColorPointer(4, GL_FLOAT, 4 * sizeof(float), BUFFER_OFFSET(0));

					/*glBindBuffer(GL_ARRAY_BUFFER, voxelNormalVBOID);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gradient.size(), &gradient[0], GL_STATIC_DRAW);
					glNormalPointer(GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));*/

					glDisableClientState(GL_VERTEX_ARRAY);
					glDisableClientState(GL_COLOR_ARRAY);
					//glDisableClientState(GL_NORMAL_ARRAY);


					_ready = true;
					_cache_active = true;
				}
			}
		}
		
	}
	
	_cached = true;
}

void DfxVolumeCache::cacheVolume2() {
	initializeOpenGLFunctions();

	if (cacheDensityTexture()) {
		glGenVertexArrays(1, &cubeVAOID);
		glGenBuffers(1, &cubeVBOID);
		glGenBuffers(1, &cubeIndicesID);

		//unit cube vertices 
		glm::vec3 vertices[8] = { glm::vec3(-0.5f,-0.5f,-0.5f),
								glm::vec3(0.5f,-0.5f,-0.5f),
								glm::vec3(0.5f, 0.5f,-0.5f),
								glm::vec3(-0.5f, 0.5f,-0.5f),
								glm::vec3(-0.5f,-0.5f, 0.5f),
								glm::vec3(0.5f,-0.5f, 0.5f),
								glm::vec3(0.5f, 0.5f, 0.5f),
								glm::vec3(-0.5f, 0.5f, 0.5f) };

		//unit cube indices
		GLushort cubeIndices[36] = { 0,5,4,
								  5,0,1,
								  3,7,6,
								  3,6,2,
								  7,4,6,
								  6,4,5,
								  2,1,3,
								  3,1,0,
								  3,0,7,
								  7,0,4,
								  6,5,2,
								  2,5,1 };

		glBindVertexArray(cubeVAOID);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBOID);
		//pass cube vertices to buffer object memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &(vertices[0].x), GL_STATIC_DRAW);

		// GL_CHECK_ERRORS
		//enable vertex attributre array for position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//pass indices to element array  buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices[0], GL_STATIC_DRAW);

		_ready = true;
		_cache_active = true;
	}
	else {
		_ready = false;
	}
	
	_cached = true;
}