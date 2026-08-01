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

#include "DfxDynaMesh.h"
#include "DfxDynaMeshGeoAdapter.h"

DfxDynaMesh::DfxDynaMesh() {

	_interface = new DfxDynaMeshGeoAdapter;
	((DfxDynaMeshGeoAdapter*)_interface)->setMesh(this);

	_cache = new DfxObjectCache;
	_cache->setMeshData(_interface);
	_cache->setAny(nullptr);

	verts  = nullptr;
	_edges = nullptr;
	vnormals = nullptr;
	fnormals = nullptr;
	_indices = nullptr;
	fids = nullptr;
	_uvs = nullptr;

	nbVerts = 0;
	nbEdges = 0;
	nbFaces = 0;
}

DfxDynaMesh::~DfxDynaMesh() {
	delete _interface;
	delete _cache;
}

void DfxDynaMesh::buildEdgeList() {

	nbEdges = nbFaces * 3;

	if (nbEdges > 0) {
		_edges = new unsigned int[nbEdges * 2];

		for (int i = 0; i < nbFaces; i++) {
			auto offset = i * 6;
			auto offsetFace = i * 3;

			auto a = _indices[offsetFace ];
			auto b = _indices[offsetFace + 1];
			auto c = _indices[offsetFace + 2];

			_edges[offset] = a;
			_edges[offset + 1] = b;

			_edges[offset + 2] = a;
			_edges[offset + 3] = c;

			_edges[offset + 4] = b;
			_edges[offset + 5] = c;
		}
	}
}

DfxObjectCache *DfxDynaMesh::cache() {
	return _cache;
}

unsigned int  DfxDynaMesh::triangleCount() {
	return nbFaces;
}

unsigned int  DfxDynaMesh::vertexCount() {
	return nbVerts;
}

unsigned int  DfxDynaMesh::edgeCount() {
	return nbEdges;
}

float *DfxDynaMesh::vertices() {
	return verts;
}

unsigned int *DfxDynaMesh::edges() {
	return _edges;
}

unsigned int  *DfxDynaMesh::indices() {
	return _indices;
}

void DfxDynaMesh::setIndices(unsigned int  *inc) {
	_indices = inc;
}

float *DfxDynaMesh::smoothNormals() {
	return vnormals;
}

float *DfxDynaMesh::faceNormals() {
	return fnormals;
}

float *DfxDynaMesh::uvs() {
	return _uvs;
}

unsigned int *DfxDynaMesh::faceIDs() {
	return fids;
}

void DfxDynaMesh::setTriangleCount(unsigned int  cnt) {
	nbFaces = cnt;
}

void DfxDynaMesh::setVertexCount(unsigned int  cnt) {
	nbVerts = cnt;
}

void DfxDynaMesh::setEdgeCount(unsigned int cnt) {
	nbEdges = cnt;
}

void DfxDynaMesh::setFaceIDs(unsigned int  *fid) {
	fids = fid;
}

void DfxDynaMesh::setVertices(float *vts) {
	verts = vts;
}

void DfxDynaMesh::setEdges(unsigned int *edgs) {
	_edges = edgs;
}

void DfxDynaMesh::setSmoothNormals(float *arr) {
	vnormals = arr;
}

void DfxDynaMesh::setFaceNormals(float *arr) {
	fnormals = arr;
}

void DfxDynaMesh::setUvs(float *arr) {
	_uvs = arr;
}

void DfxDynaMesh::calcNormals() {

	for (int i = 0; i < triangleCount(); i++) {

		int i_of = i * 3;
		int v1_of = _indices[i_of] * 3;
		int v2_of = _indices[i_of + 1] * 3;
		int v3_of = _indices[i_of + 2] * 3;

		glm::vec3 a(verts[v1_of], verts[v1_of + 1], verts[v1_of + 2]);
		glm::vec3 b(verts[v2_of], verts[v2_of + 1], verts[v2_of + 2]);
		glm::vec3 c(verts[v3_of], verts[v3_of + 1], verts[v3_of + 2]);

		glm::vec3 u(b - a);
		glm::vec3 v(c - a);

		glm::vec3 fnorm = glm::cross(u, v);
		// glm::vec3 normi = glm::normalize(fnorm);

		vnormals[v1_of] += fnorm.x;
		vnormals[v1_of + 1] += fnorm.y;
		vnormals[v1_of + 2] += fnorm.z;

		vnormals[v2_of] += fnorm.x;
		vnormals[v2_of + 1] += fnorm.y;
		vnormals[v2_of + 2] += fnorm.z;

		vnormals[v3_of] += fnorm.x;
		vnormals[v3_of + 1] += fnorm.y;
		vnormals[v3_of + 2] += fnorm.z;
	}

	for (int i = 0; i < vertexCount(); i++) {
		int i_of = i * 3;

		glm::vec3 a(vnormals[i_of], vnormals[i_of + 1], vnormals[i_of + 2]);
		a = glm::normalize(a);

		vnormals[i_of] = a.x;
		vnormals[i_of + 1] = a.y;
		vnormals[i_of + 2] = a.z;
	}
}

void DfxDynaMesh::clearVertices() {
	delete[] verts;
	verts = nullptr;
	nbVerts = 0;
}

void DfxDynaMesh::clearIndices() {
	delete[] _indices;
	_indices = nullptr;
	nbFaces = 0;
}

void DfxDynaMesh::clearSmoothNormals() {
	delete[] vnormals;
	vnormals = nullptr;
}

void DfxDynaMesh::clearFaceNormals() {
	delete[] fnormals;
	fnormals = nullptr;
}

void DfxDynaMesh::clearFaceIDs() {
	delete[] fids;
	fids = nullptr;
}

void DfxDynaMesh::clearUvs() {
	delete[] _uvs;
	_uvs = nullptr;
}

void DfxDynaMesh::clearAll() {
	clearVertices();
	clearIndices();
	clearSmoothNormals();
	clearFaceNormals();
	clearFaceIDs();
	clearUvs();
}

void DfxDynaMesh::copyMesh(deepsg::SgGeoNode *geo) {
	clearAll();

	auto adapter = geo->getGeometryAdapter();

	if (adapter->getVertexCount() > 0) {
		verts = new float[adapter->getVertexCount() * 3];
		vnormals = new float[adapter->getVertexCount() * 3];
		_uvs     = new float[adapter->getVertexCount() * 2];
		nbVerts = adapter->getVertexCount();

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;
			int counterUv = i * 2;

			auto v = adapter->getVertex(i);
			auto n = adapter->getSmoothNormal(i);
			auto u = adapter->getUV(i);

			verts[counter] = v.x;
			verts[counter + 1] = v.y;
			verts[counter + 2] = v.z;

			vnormals[counter] = n.x;
			vnormals[counter + 1] = n.y;
			vnormals[counter + 2] = n.z;

			_uvs[counterUv] = u.x;
			_uvs[counterUv + 1] = u.y;
		}
	}

	auto triCount = adapter->getTriangleIndexListSize();
	if (triCount > 0) {
		_indices = adapter->getTriangleIndexList();
		nbFaces = triCount;

		fnormals = new float[nbFaces * 3];
		fids = new unsigned int[nbFaces];

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;

			auto n = adapter->getFlatNormal(i);
			auto f = adapter->getPolygonSurfaceID(i);

			fnormals[counter] = n.x;
			fnormals[counter + 1] = n.y;
			fnormals[counter + 2] = n.z;

			fids[i] = f;
		}
	}
}

void DfxDynaMesh::copyMesh(Mesh *mesh, std::string objPath) {
	clearAll();

	if (mesh->nbVerts > 0) {

		verts = new float[mesh->nbVerts * 3];
		vnormals = new float[mesh->nbVerts * 3];
		nbVerts = mesh->nbVerts;

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;
			int counterUv = i * 2;

			auto v = mesh->verts[i];
			auto n = mesh->normals[i];

			verts[counter] = v.x;
			verts[counter + 1] = v.y;
			verts[counter + 2] = v.z;

			vnormals[counter] = n.x;
			vnormals[counter + 1] = n.y;
			vnormals[counter + 2] = n.z;

		}
	}

	if (mesh->nbFaces > 0) {

		std::vector<deepsg::SgGeoAdapterInterface::Triangle> triangles;

		for (size_t inx = 0; inx < mesh->nbFaces; inx++) {
			std::vector<size_t> faceIndices;

			auto index = inx * 4;

			faceIndices.push_back(mesh->indices[index]);
			faceIndices.push_back(mesh->indices[index + 1]);
			faceIndices.push_back(mesh->indices[index + 2]);
			faceIndices.push_back(mesh->indices[index + 3]);

			auto tempTriangles = deepsg::SgGeoAdapterInterface::polygonToTriangles(mesh->verts, 4, faceIndices, inx);

			for (auto tri : tempTriangles) {
				triangles.push_back(tri);
			}
		}
		
		size_t surf_id = 0;
		if (!objPath.empty()) {
			DfxSurfaceDictionary::getInstance()->create(objPath);
			surf_id = DfxSurfaceDictionary::ID_LIMIT + 1;
			DfxSurfaceDictionary::getInstance()->insert(objPath, surf_id, "default");
		}

		if (triangles.size() > 0) {
			nbFaces = triangles.size();

			_indices = new unsigned int[triangles.size() * 3];
			_uvs = new float[triangles.size() * 3 * 2];
			fids = new unsigned int[nbFaces];
			fnormals = new float[triangles.size() * 3];

			unsigned int counter = 0;
			unsigned int indexCounter = 0;
			unsigned int uvCounter = 0;
			for (auto tri : triangles) {

				_indices[counter] = tri.a;
				_indices[counter + 1] = tri.b;
				_indices[counter + 2] = tri.c;

				fnormals[counter] = 0.f;
				fnormals[counter + 1] = 0.f;
				fnormals[counter + 2] = 0.f;

				_uvs[uvCounter] = 0.f;
				_uvs[uvCounter + 1] = 0.f;

				_uvs[uvCounter + 2] = 0.f;
				_uvs[uvCounter + 3] = 0.f;

				_uvs[uvCounter + 4] = 0.f;
				_uvs[uvCounter + 5] = 0.f;

				fids[indexCounter] = surf_id;

				uvCounter += 6;
				counter += 3;
				indexCounter++;
			}

			// calcNormals();
		}
	}
}

deepsg::SgGeoAdapterInterface *DfxDynaMesh::getGeometryAdapter() {
	return _interface;
}