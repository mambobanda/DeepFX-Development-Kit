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

#include <DsMesh.h>
#include <SgGeoAdapterInterface.h>
#include "DfxGeoAdapterType.h"
#include "DfxResourcePtr.h"

class DfxDeepSimplexGeoAdapter: public deepsg::SgGeoAdapterInterface {
public:
	static const size_t AdapterType = 0xff;
private:
	DfxResourcePtr *_mesh;
public:

	void setMesh(DfxResourcePtr *);
	DfxResourcePtr *getMesh();

	size_t getVertexCount() override;
	glm::vec3 getVertex(size_t index) override;
	glm::vec3 getSmoothNormal(size_t index) override;
	glm::vec3 getFlatNormal(size_t index) override;
	glm::vec2 getUV(size_t index) override;

	float* getFlatVertexList() override;
	float* getInterleavedVertexList(float *vertexBuffer = nullptr) override;
	float* getFlatInterleavedVertexList(float *vertexBuffer = nullptr) override;
	unsigned int *getTriangleIndexList() override;
	unsigned int *getTriangleSourceFaceIndexList() override;
	unsigned int *getFlatTriangleIndexList() override;
	unsigned int getTriangleIndexListSize() override;
	unsigned int *getEdgeIndexList() override;
	unsigned int getEdgeIndexListSize() override;

	size_t getEdgeCount() override;
	std::pair<size_t, size_t> getEdgeVerticeIndices(size_t inx) override;

	size_t getPolygonCount() override;
	size_t getPolygonSurfaceID(size_t) override;
	int getPolygonDegree(size_t) override;
	size_t getPolygonVertexIndex(int face, int vertex) override;

	virtual size_t islandCount() override;
	virtual unsigned int *island(size_t) override;

	void *meshPtr() override;
	size_t type() override;

	std::unordered_map<size_t, glm::vec2> getPolygonUVMap(size_t) override;

	// Experimental
	void getTriangleSelectedIndexList(std::vector<size_t> uids, std::vector<unsigned int> &output);
};