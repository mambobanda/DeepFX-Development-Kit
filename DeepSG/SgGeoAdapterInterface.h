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

#include <DsTesselator.h>
#include <glm\vec3.hpp>
#include <vector>
#include <unordered_map>
#include <DsMesh.h>

namespace deepsg {

	class SgGeoAdapterInterface {
	public:

		class Triangle {
		public:

			size_t a;
			size_t b;
			size_t c;

			size_t faceIndex;
		};

	public:

		virtual ~SgGeoAdapterInterface();
		virtual size_t getVertexCount() = 0;
		virtual glm::vec3 getVertex(size_t index) = 0;
		virtual glm::vec3 getSmoothNormal(size_t index) = 0;
		virtual glm::vec3 getFlatNormal(size_t index) = 0;
		virtual glm::vec2 getUV(size_t index) = 0;

		virtual size_t getEdgeCount() = 0;
		virtual std::pair<size_t, size_t> getEdgeVerticeIndices(size_t inx) = 0;

		virtual size_t getPolygonCount() = 0;
		virtual size_t getPolygonSurfaceID(size_t) = 0;
		virtual int getPolygonDegree(size_t) = 0;
		virtual size_t getPolygonVertexIndex(int face, int vertex) = 0;

		virtual float* getFlatVertexList() = 0;
		virtual float* getInterleavedVertexList(float *vertexBuffer = nullptr) = 0;
		virtual float* getFlatInterleavedVertexList(float *vertexBuffer = nullptr) = 0;
		virtual unsigned int *getTriangleIndexList() = 0;
		virtual unsigned int *getTriangleSourceFaceIndexList() = 0;
		virtual unsigned int *getFlatTriangleIndexList() = 0;
		virtual unsigned int getTriangleIndexListSize()  = 0;
		virtual unsigned int *getEdgeIndexList() = 0;
		virtual unsigned int getEdgeIndexListSize() = 0;

		virtual size_t islandCount() = 0;
		virtual unsigned int *island(size_t) = 0;

		virtual void *meshPtr() = 0;
		
		virtual size_t type() = 0;

		virtual std::unordered_map<size_t, glm::vec2> getPolygonUVMap(size_t) = 0;

		static std::vector<SgGeoAdapterInterface::Triangle> polygonToTriangles(glm::vec3 *inverts, int degree, std::vector<size_t> &indices, size_t faceIndex);
		static std::vector<SgGeoAdapterInterface::Triangle> polygonToTriangles(float *inverts, int degree, std::vector<size_t> &indices, size_t faceIndex);
		std::vector<SgGeoAdapterInterface::Triangle> polygonToTriangles(size_t);

		virtual std::vector<glm::vec3> calculateSmoothNormals(float *vertices = nullptr);
	};

}