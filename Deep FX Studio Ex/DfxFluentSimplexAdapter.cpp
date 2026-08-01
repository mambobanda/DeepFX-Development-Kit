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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxFluentSimplexAdapter.h"

void DfxFluentSimplexAdapter::setMesh(DfxFluentSimplexObject *m) {
	mesh = m;
}

DfxFluentSimplexObject *DfxFluentSimplexAdapter::getMesh() {
	return mesh;
}

size_t DfxFluentSimplexAdapter::getVertexCount() {
	return mesh->vertexCount();
}

glm::vec3 DfxFluentSimplexAdapter::getVertex(size_t index) {
	unsigned int offset = index * 3;
	auto v = mesh->vertices();
	return glm::vec3(v[offset], v[offset + 1], v[offset + 2]);
}

glm::vec3 DfxFluentSimplexAdapter::getSmoothNormal(size_t index) {
	unsigned int offset = index * 3;
	auto n = mesh->smoothNormals();
	return glm::vec3(n[offset], n[offset + 1], n[offset + 2]);
}

glm::vec3 DfxFluentSimplexAdapter::getFlatNormal(size_t index) {
	unsigned int offset = index * 3;
	auto n = mesh->faceNormals();
	return glm::vec3(n[offset], n[offset + 1], n[offset + 2]);
}

glm::vec2 DfxFluentSimplexAdapter::getUV(size_t index) {
	unsigned int offset = index * 2;
	auto uv = mesh->uvs();
	return glm::vec2(uv[offset], uv[offset + 1]);
}

float* DfxFluentSimplexAdapter::getFlatVertexList() {
	float *vlist = new float[mesh->vertexCount() * 3];

	size_t counter = 0;
	for (int i = 0; i < mesh->vertexCount(); i++) {

		vlist[counter] = mesh->vertices()[counter];
		vlist[counter + 1] = mesh->vertices()[counter + 1];
		vlist[counter + 2] = mesh->vertices()[counter + 2];

		counter += 3;
	}

	return vlist;

}

float* DfxFluentSimplexAdapter::getInterleavedVertexList(float *vertexBuffer) {
	float *vlist = new float[mesh->vertexCount() * 8];

	#pragma omp parallel default(shared)
	{
		#pragma omp for schedule(static)
		for (int inx = 0; inx < mesh->vertexCount(); inx++) {

			size_t counter = inx * 8;
			size_t buffer_counter = inx * 3;
			size_t buffer_uv_counter = inx * 2;

			auto v = mesh->vertices()[inx];

			if (vertexBuffer != nullptr) {
				vlist[counter] = vertexBuffer[buffer_counter];
				vlist[counter + 1] = vertexBuffer[buffer_counter + 1];
				vlist[counter + 2] = vertexBuffer[buffer_counter + 2];
			}
			else {
				vlist[counter] = mesh->vertices()[buffer_counter];
				vlist[counter + 1] = mesh->vertices()[buffer_counter + 1];
				vlist[counter + 2] = mesh->vertices()[buffer_counter + 2];
			}

			vlist[counter + 3] = mesh->smoothNormals()[buffer_counter];
			vlist[counter + 4] = mesh->smoothNormals()[buffer_counter + 1];
			vlist[counter + 5] = mesh->smoothNormals()[buffer_counter + 2];

			vlist[counter + 6] = 0.f;
			vlist[counter + 7] = 0.f;
		}
	}

	return vlist;
}

float* DfxFluentSimplexAdapter::getFlatInterleavedVertexList(float *vertexBuffer) {
	float *vlist = new float[mesh->triangleCount() * 3 * 3 * 8];

	auto indexList = mesh->indices();

	size_t counter = 0;
	size_t buffer_counter = 0;
	glm::vec3 normal(0);
	for (size_t inx = 0; inx < getTriangleIndexListSize(); inx++) {

		if ((inx % 3) == 0) {
			glm::vec3 u(getVertex(indexList[inx + 1]) - getVertex(indexList[inx]));
			glm::vec3 v(getVertex(indexList[inx + 2]) - getVertex(indexList[inx]));

			normal = glm::normalize(glm::cross(u, v));
		}

		if (vertexBuffer != nullptr) {
			vlist[counter] = vertexBuffer[buffer_counter];
			vlist[counter + 1] = vertexBuffer[buffer_counter + 1];
			vlist[counter + 2] = vertexBuffer[buffer_counter + 2];
		}
		else {
			vlist[counter] = getVertex(indexList[inx]).x;
			vlist[counter + 1] = getVertex(indexList[inx]).y;
			vlist[counter + 2] = getVertex(indexList[inx]).z;
		}

		vlist[counter + 3] = normal.x;
		vlist[counter + 4] = normal.y;
		vlist[counter + 5] = normal.z;

		vlist[counter + 6] = 0.f;
		vlist[counter + 7] = 0.f;

		counter += 8;
		buffer_counter += 3;
	}

	return vlist;
}

unsigned int *DfxFluentSimplexAdapter::getTriangleIndexList() {
	unsigned int *e_indices = new unsigned int[mesh->triangleCount() * 3];

	for (size_t inx = 0; inx < mesh->triangleCount() * 3; inx++) {
		e_indices[inx] = mesh->indices()[inx];
	}

	return e_indices;
}

unsigned int *DfxFluentSimplexAdapter::getTriangleSourceFaceIndexList() {
	unsigned int *e_indices = new unsigned int[mesh->triangleCount()];

	for (size_t inx = 0; inx < mesh->triangleCount(); inx++) {
		e_indices[inx] = inx;
	}

	return e_indices;
}

unsigned int *DfxFluentSimplexAdapter::getFlatTriangleIndexList() {
	unsigned int *e_indices = new unsigned int[mesh->triangleCount() * 3];

	for (size_t inx = 0; inx < mesh->triangleCount() * 3; inx++) {
		e_indices[inx] = inx;
	}

	return e_indices;
}

unsigned int DfxFluentSimplexAdapter::getTriangleIndexListSize() {
	return mesh->triangleCount() * 3;
}

unsigned int *DfxFluentSimplexAdapter::getEdgeIndexList() {
	return mesh->edges();
}

unsigned int DfxFluentSimplexAdapter::getEdgeIndexListSize() {
	return mesh->edgeCount() * 2;
}

size_t DfxFluentSimplexAdapter::getEdgeCount() {
	return mesh->edgeCount();
}

std::pair<size_t, size_t> DfxFluentSimplexAdapter::getEdgeVerticeIndices(size_t inx) {
	return std::pair<size_t, size_t>();
}

size_t DfxFluentSimplexAdapter::getPolygonCount() {
	return mesh->triangleCount();
}

size_t DfxFluentSimplexAdapter::getPolygonSurfaceID(size_t index) {
	return mesh->faceIDs()[index];
}

int DfxFluentSimplexAdapter::getPolygonDegree(size_t) {
	return 3;
}

size_t DfxFluentSimplexAdapter::getPolygonVertexIndex(int face, int vertex) {
	int offset = face * 3;
	return mesh->indices()[offset + vertex];
}

size_t DfxFluentSimplexAdapter::islandCount() {
	return 0;
}

unsigned int *DfxFluentSimplexAdapter::island(size_t) {
	return nullptr;
}

void *DfxFluentSimplexAdapter::meshPtr() {
	return mesh;
}

size_t DfxFluentSimplexAdapter::type() {
	return DfxGeoAdapterType::DYNA_MESH;
}

std::unordered_map<size_t, glm::vec2> DfxFluentSimplexAdapter::getPolygonUVMap(size_t index) {
	std::unordered_map<size_t, glm::vec2> output;

	auto offset = index * 3;
	auto offsetUv = index * 6;

	auto a = mesh->indices()[offset];
	auto b = mesh->indices()[offset + 1];
	auto c = mesh->indices()[offset + 2];

	output[a] = glm::vec2(mesh->uvs()[offsetUv], mesh->uvs()[offsetUv + 1]);
	output[b] = glm::vec2(mesh->uvs()[offsetUv + 2], mesh->uvs()[offsetUv + 3]);
	output[c] = glm::vec2(mesh->uvs()[offsetUv + 4], mesh->uvs()[offsetUv + 5]);

	return output;
}

void DfxFluentSimplexAdapter::getTriangleSelectedIndexList(std::vector<size_t> indices, std::vector<unsigned int> &output) {

	std::unique_ptr<unsigned int> tri_indices(getTriangleIndexList());

	for (unsigned int i = 0; i < indices.size(); i++) {
		auto offset = indices[i] * 3;
		output.push_back(tri_indices.get()[offset]);
		output.push_back(tri_indices.get()[offset + 1]);
		output.push_back(tri_indices.get()[offset + 2]);
	}

}