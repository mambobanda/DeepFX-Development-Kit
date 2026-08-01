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

#include <vector>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/normal.hpp>
#include <iostream>
#include "Dfx3dMeshData.h"

std::atomic<size_t> Dfx3dMeshData::ID_GEN(1);

Dfx3dMeshData::Dfx3dMeshData() {
	verticies = nullptr;
	normals = nullptr;
	colors = nullptr;
	uvs = nullptr;
	indices   = nullptr;

	vertexCount = 0;
	indexCount = 0;

	id = ID_GEN++;
}

Dfx3dMeshData::~Dfx3dMeshData() {
	clearVertexBuffer();
	clearIndexBuffer();
}

void Dfx3dMeshData::clearVertexBuffer() {
	delete[] verticies;
	delete[] normals;
	delete[] colors;
	delete[] uvs;

	verticies = nullptr;
	normals = nullptr;
	colors = nullptr;
	uvs = nullptr;

	vertexCount = 0;
}

void Dfx3dMeshData::clearIndexBuffer() {
	delete[] indices;

	indices = nullptr;

	indexCount = 0;
}

void Dfx3dMeshData::resizeVertexBuffer(size_t sz) {

	clearVertexBuffer();

	verticies = new glm::vec3[sz]();
	normals   = new glm::vec3[sz]();
	colors  = new glm::vec3[sz]();
	uvs = new glm::vec2[sz]();

	vertexCount = sz;
}

void Dfx3dMeshData::resizeIndexBuffer(size_t cnt) {

	clearIndexBuffer();
	indices = new size_t[cnt]();

	indexCount = cnt;
}

glm::vec3 Dfx3dMeshData::getVertex(size_t index) {
	if (vertexCount >= 0 && index < vertexCount) {
		return verticies[index];
	}
	else {
		return verticies[0];
	}
}

DfxEntityPropertyTypes::Float3 Dfx3dMeshData::_getVertex(size_t index) {
	DfxEntityPropertyTypes::Float3 f3;

	if (vertexCount >= 0 && index < vertexCount) {
		f3.x = verticies[index].x;
		f3.y = verticies[index].y;
		f3.z = verticies[index].z;
	}
	else
	{
		f3.x = 0.f;
		f3.y = 0.f;
		f3.z = 0.f;
	}

	return f3;
}

void Dfx3dMeshData::setVertex(size_t index, glm::vec3 pos) {

	verticies[index] = pos;
}

void Dfx3dMeshData::setVertex(size_t index, float x, float y, float z) {

	verticies[index].x = x;
	verticies[index].y = y;
	verticies[index].z = z;
}

void Dfx3dMeshData::_setVertex(size_t index, float x, float y, float z) {
	setVertex(index, x, y, z);
}

glm::vec2 Dfx3dMeshData::getUV(size_t index) {
	
	if (vertexCount >= 0 && index < vertexCount) {
		return uvs[index];
	}
	else {
		return uvs[0];
	}
}

DfxEntityPropertyTypes::Float3 Dfx3dMeshData::_getUV(size_t index) {
	DfxEntityPropertyTypes::Float3 f2;

	if (vertexCount >= 0 && index < vertexCount) {
		f2.x = uvs[index].x;
		f2.y = uvs[index].y;
	}
	else
	{
		f2.x = 0.f;
		f2.y = 0.f;
	}

	return f2;
}

void Dfx3dMeshData::setUV(size_t index, glm::vec2 uv) {
	uvs[index] = uv;
}

void Dfx3dMeshData::setUV(size_t index, float x, float y, float z) {
	verticies[index].x = x;
	verticies[index].y = y;
	verticies[index].z = z;
}

void Dfx3dMeshData::_setUV(size_t index, float x, float y, float z) {
	setVertex(index, x, y, z);
}

glm::vec3 Dfx3dMeshData::getNormal(size_t index) {
	if (vertexCount >= 0 && index < vertexCount) {
		return normals[index];
	}
	else {
		return normals[0];
	}
}

DfxEntityPropertyTypes::Float3 Dfx3dMeshData::_getNormal(size_t index) {
	DfxEntityPropertyTypes::Float3 f3;

	if (vertexCount >= 0 && index < vertexCount) {
		f3.x = normals[index].x;
		f3.y = normals[index].y;
		f3.z = normals[index].z;
	}
	else {
		f3.x = normals[0].x;
		f3.y = normals[0].y;
		f3.z = normals[0].z;
	}

	return f3;
}

void Dfx3dMeshData::setNormal(size_t index, glm::vec3 norm) {

	normals[index] = norm;
}

void Dfx3dMeshData::setNormal(size_t index, float x, float y, float z) {

	verticies[index].x = x;
	verticies[index].y = y;
	verticies[index].z = z;
}

void Dfx3dMeshData::_setNormal(size_t index, float x, float y, float z) {
	setNormal(index, x, y, z);
}

size_t Dfx3dMeshData::getIndex(size_t inx) {
	return indices[inx];
}

void Dfx3dMeshData::setIndex(size_t inx, size_t val) {
	indices[inx] = val;
}

glm::vec3 Dfx3dMeshData::getColor(size_t index) {
	if (vertexCount >= 0 && index < vertexCount) {
		return colors[index];
	}
	else {
		return colors[0];
	}
}

DfxEntityPropertyTypes::Float3 Dfx3dMeshData::_getColor(size_t index) {
	DfxEntityPropertyTypes::Float3 f3;

	if (vertexCount >= 0 && index < vertexCount) {
		f3.x = colors[index].x;
		f3.y = colors[index].y;
		f3.z = colors[index].z;
	}
	else {
		f3.x = colors[0].x;
		f3.y = colors[0].y;
		f3.z = colors[0].z;
	}

	return f3;
}

void Dfx3dMeshData::setColor(size_t index, glm::vec3 col) {

	colors[index].x = col.x;
	colors[index].y = col.y;
	colors[index].z = col.z;
}

void Dfx3dMeshData::setColor(size_t index, float r, float g, float b) {

	colors[index].x = r;
	colors[index].y = g;
	colors[index].z = b;
}

void Dfx3dMeshData::_setColor(size_t index, float x, float y, float z) {
	setColor(index, x, y, z);
}

size_t Dfx3dMeshData::getVertexCount() {
	return vertexCount;
}

size_t Dfx3dMeshData::getIndexCount() {
	return indexCount;
}

glm::vec3 *Dfx3dMeshData::getVertexBuffer() {
	return verticies;
}

size_t *Dfx3dMeshData::getIndexBuffer() {
	return indices;
}

unsigned int Dfx3dMeshData::getVertexBufferBytes() {
	return vertexBufferBytes;
}

unsigned int Dfx3dMeshData::getIndexBufferBytes() {
	return indexBufferBytes;
}

void Dfx3dMeshData::copy( Dfx3dMeshData &cp_msh) {

	resizeVertexBuffer(cp_msh.getVertexCount());
	resizeIndexBuffer(cp_msh.getIndexCount());

	for (int inx = 0; inx < cp_msh.getVertexCount(); inx++) {
		setVertex(inx, cp_msh.getVertex(inx));
		setNormal(inx, cp_msh.getNormal(inx));
		setColor(inx, cp_msh.getColor(inx));
	}

	for (int inx = 0; inx < cp_msh.getIndexCount(); inx++) {
		setIndex(inx, cp_msh.getIndex(inx));
	}
}

void Dfx3dMeshData::calculateVertexNormals() {

	if (getVertexCount() > 0) {

		std::vector<glm::vec3> normals;
		std::vector<uint32_t> nbcount;

		normals.resize(getVertexCount());
		nbcount.resize(getVertexCount());

		size_t counter = 0;
		for (size_t inx = 0; inx < getIndexCount() / 3; inx++) {
			glm::vec3 a = getVertex(getIndex(counter));
			glm::vec3 b = getVertex(getIndex(counter + 1));
			glm::vec3 c = getVertex(getIndex(counter + 2));

			glm::vec3 norm = glm::triangleNormal(a, b, c);

			normals[getIndex(counter)] += norm;
			normals[getIndex(counter + 1)] += norm;
			normals[getIndex(counter + 2)] += norm;

			nbcount[getIndex(counter)] += 1;
			nbcount[getIndex(counter + 1)] += 1;
			nbcount[getIndex(counter + 2)] += 1;

			counter += 3;
		}

		for (size_t inx = 0; inx < getVertexCount() / 3; inx++) {

			if (nbcount[inx] > 0) {
				glm::vec3 norm_avg = normals[inx] / (float)nbcount[inx];

				setNormal(inx, glm::normalize(norm_avg));
			}
			else {
				setNormal(inx, glm::vec3());
			}
		}
	}
}

size_t Dfx3dMeshData::getID() const {
	return id;
}

void Dfx3dMeshData::transformData(glm::mat4 &mat) {

	for (size_t inx = 0; inx < getVertexCount(); inx++) {
		glm::vec3 npos = glm::vec3(mat * glm::vec4(getVertex(inx), 1.f));
		setVertex(inx, npos);
	}
}