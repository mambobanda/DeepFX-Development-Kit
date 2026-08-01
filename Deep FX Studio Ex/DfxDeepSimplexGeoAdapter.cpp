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

#include "DfxDeepSimplexGeoAdapter.h"

DfxResourcePtr *DfxDeepSimplexGeoAdapter::getMesh() {
	return _mesh;
}

void DfxDeepSimplexGeoAdapter::setMesh(DfxResourcePtr *m) {
	_mesh = m;
}

size_t DfxDeepSimplexGeoAdapter::getVertexCount() {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->vertices().size();
}

glm::vec3 DfxDeepSimplexGeoAdapter::getVertex(size_t index) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->vertices()[index]->position();
}

glm::vec3 DfxDeepSimplexGeoAdapter::getSmoothNormal(size_t index) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->vertices()[index]->getProperty<glm::vec3>("smooth_normals");
}

glm::vec3 DfxDeepSimplexGeoAdapter::getFlatNormal(size_t index) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces()[index]->getProperty<glm::vec3>("face_normals");
}

glm::vec2 DfxDeepSimplexGeoAdapter::getUV(size_t index) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return  mesh->vertices()[index]->getProperty<glm::vec2>("uvs");
}

size_t DfxDeepSimplexGeoAdapter::getEdgeCount() {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->edges().size();
}

float* DfxDeepSimplexGeoAdapter::getFlatVertexList() {
	auto mesh = _mesh->resource<ds::Mesh>();
	float *vlist = new float[mesh->vertices().size() * 3];

	size_t counter = 0;
	for (auto v : mesh->vertices()) {
		
		vlist[counter] = v->position().x;
		vlist[counter + 1] = v->position().y;
		vlist[counter + 2] = v->position().z;
	
		counter += 3;
	}

	return vlist;
}

float* DfxDeepSimplexGeoAdapter::getInterleavedVertexList(float *vertexBuffer) {
	auto mesh = _mesh->resource<ds::Mesh>();
	float *vlist = new float[mesh->vertices().size() * 8];

	#pragma omp parallel default(shared)
	{
		#pragma omp for schedule(static)
		for (int inx = 0; inx < mesh->vertices().size(); inx++) {

			size_t counter = inx * 8;
			size_t buffer_counter = inx * 3;

			auto v = mesh->vertices()[inx];
		
			if (vertexBuffer != nullptr) {
				vlist[counter] = vertexBuffer[buffer_counter];
				vlist[counter + 1] = vertexBuffer[buffer_counter + 1];
				vlist[counter + 2] = vertexBuffer[buffer_counter + 2];
			}
			else {
				vlist[counter] = v->position().x;
				vlist[counter + 1] = v->position().y;
				vlist[counter + 2] = v->position().z;
			}

			auto normal = v->getProperty<glm::vec3>("smooth_normals");
			vlist[counter + 3] = normal.x;
			vlist[counter + 4] = normal.y;
			vlist[counter + 5] = normal.z;

			auto uvs = v->getProperty<glm::vec2>("uvs");
			vlist[counter + 6] = uvs.x;
			vlist[counter + 7] = uvs.y;

		}
	}

	return vlist;
}

float *DfxDeepSimplexGeoAdapter::getFlatInterleavedVertexList(float *vertexBuffer) {
	auto mesh = _mesh->resource<ds::Mesh>();
	float *vlist = new float[mesh->triangleIndexMap().triangleCount() * 3 * 3 * 8];

	auto indexList = mesh->triangleIndexMap().toIndexArray();

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
			vlist[counter]	   = vertexBuffer[buffer_counter];
			vlist[counter + 1] = vertexBuffer[buffer_counter + 1];
			vlist[counter + 2] = vertexBuffer[buffer_counter + 2];
		}
		else {
			vlist[counter]	   = getVertex(indexList[inx]).x;
			vlist[counter + 1] = getVertex(indexList[inx]).y;
			vlist[counter + 2] = getVertex(indexList[inx]).z;
		}
		
		vlist[counter + 3] = normal.x;
		vlist[counter + 4] = normal.y;
		vlist[counter + 5] = normal.z;

		vlist[counter + 6] = getUV(indexList[inx]).x;
		vlist[counter + 7] = getUV(indexList[inx]).y;
		
		counter += 8;
		buffer_counter += 3;
	}

	delete[] indexList;

	return vlist;
}

unsigned int *DfxDeepSimplexGeoAdapter::getTriangleIndexList() {
	auto mesh = _mesh->resource<ds::Mesh>();
	auto indexList = mesh->triangleIndexMap().toIndexArray(mesh->faces());

	return indexList;
}

unsigned int *DfxDeepSimplexGeoAdapter::getTriangleSourceFaceIndexList() {
	auto mesh = _mesh->resource<ds::Mesh>();
	auto faceIndexList = mesh->triangleIndexMap().sourceFaceIndexArray(mesh->faces());

	return faceIndexList;
}

unsigned int DfxDeepSimplexGeoAdapter::getTriangleIndexListSize() {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->triangleIndexMap().triangleCount() * 3;
}

std::pair<size_t, size_t> DfxDeepSimplexGeoAdapter::getEdgeVerticeIndices(size_t inx) {
	auto mesh = _mesh->resource<ds::Mesh>();
	std::pair<size_t, size_t> p(mesh->edges()[inx]->v00()->getIndex(),
								mesh->edges()[inx]->v01()->getIndex());
	return p;
}

size_t DfxDeepSimplexGeoAdapter::getPolygonCount() {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces().size();
}

size_t DfxDeepSimplexGeoAdapter::getPolygonSurfaceID(size_t inx) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces()[inx]->getSurfID();
}

int DfxDeepSimplexGeoAdapter::getPolygonDegree(size_t inx) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces()[inx]->verticesCount();
}

size_t DfxDeepSimplexGeoAdapter::getPolygonVertexIndex(int face, int vertex) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces()[face]->vertices()[vertex]->getIndex();
}

unsigned int *DfxDeepSimplexGeoAdapter::getEdgeIndexList() {
	unsigned int *e_indices = new unsigned int[getEdgeCount() * 2];

	for (size_t inx = 0; inx < getEdgeCount(); inx++) {
		size_t counter = inx * 2;
		auto p = getEdgeVerticeIndices(inx);

		e_indices[counter] = p.first;
		e_indices[counter + 1] = p.second;
	}

	return e_indices;
}

unsigned int *DfxDeepSimplexGeoAdapter::getFlatTriangleIndexList() {
	auto mesh = _mesh->resource<ds::Mesh>();
	unsigned int *e_indices = new unsigned int[mesh->triangleIndexMap().triangleCount() * 3];

	for (size_t inx = 0; inx < mesh->triangleIndexMap().triangleCount() * 3; inx++) {
		e_indices[inx] = inx;
	}

	return e_indices;
}

unsigned int DfxDeepSimplexGeoAdapter::getEdgeIndexListSize() {
	return getEdgeCount() * 2;
}

size_t DfxDeepSimplexGeoAdapter::islandCount() {
	return 0;
}

unsigned int *DfxDeepSimplexGeoAdapter::island(size_t) {
	return nullptr;
}

void *DfxDeepSimplexGeoAdapter::meshPtr() {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh;
}

size_t DfxDeepSimplexGeoAdapter::type() {
	return DfxGeoAdapterType::DEEP_SIMPLEX;
}

std::unordered_map<size_t, glm::vec2> DfxDeepSimplexGeoAdapter::getPolygonUVMap(size_t index) {
	auto mesh = _mesh->resource<ds::Mesh>();
	return mesh->faces()[index]->mapUVS();
}

void DfxDeepSimplexGeoAdapter::getTriangleSelectedIndexList(std::vector<size_t> uids, std::vector<unsigned int> &output) {
	auto mesh = _mesh->resource<ds::Mesh>();
	mesh->triangleIndexMap().toIndexArray(uids, output);
}
