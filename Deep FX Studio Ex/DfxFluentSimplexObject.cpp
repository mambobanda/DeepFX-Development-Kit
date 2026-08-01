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

#include <DfxBounds.h>
#include <glm/gtx/transform.hpp>
#include "DfxFluentSimplexObject.h"
#include "DfxFluentSimplexAdapter.h"

DfxFluentSimplexObject::DfxFluentSimplexObject() {

	_interface = new DfxFluentSimplexAdapter;
	((DfxFluentSimplexAdapter*)_interface)->setMesh(this);

	_cache = new DfxObjectCache;
	_cache->setMeshData(_interface);
	_cache->setAny(nullptr);

	verts = nullptr;
	_edges = nullptr;
	vnormals = nullptr;
	fnormals = nullptr;
	_indices = nullptr;
	fids = nullptr;
	_uvs = nullptr;

	_vertexVersion = 1;
	_uvVersion = 1;
	_vertexNormalVersion = 1;
	_materialIDVersion = 1;
	_indexVersion = 1;
	_topolgyVersion = 1;

	nbVerts = 0;
	nbEdges = 0;
	nbFaces = 0;
}

DfxFluentSimplexObject::~DfxFluentSimplexObject() {
	_cache->clear();
	clearAll();

	delete _interface;
	delete _cache;
}

void DfxFluentSimplexObject::buildEdgeList() {

	nbEdges = nbFaces * 3;

	if (nbEdges > 0) {
		_edges = new unsigned int[nbEdges * 2];

		for (int i = 0; i < nbFaces; i++) {
			auto offset = i * 6;
			auto offsetFace = i * 3;

			auto a = _indices[offsetFace];
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

DfxObjectCache *DfxFluentSimplexObject::cache() {
	return _cache;
}

unsigned int  DfxFluentSimplexObject::triangleCount() {
	return nbFaces;
}

unsigned int  DfxFluentSimplexObject::vertexCount() {
	return nbVerts;
}

unsigned int  DfxFluentSimplexObject::edgeCount() {
	return nbEdges;
}

float *DfxFluentSimplexObject::vertices() {
	return verts;
}

unsigned int *DfxFluentSimplexObject::edges() {
	return _edges;
}

unsigned int  *DfxFluentSimplexObject::indices() {
	return _indices;
}

void DfxFluentSimplexObject::setIndices(unsigned int  *inc) {
	_indices = inc;
}

float *DfxFluentSimplexObject::smoothNormals() {
	return vnormals;
}

float *DfxFluentSimplexObject::faceNormals() {
	return fnormals;
}

float *DfxFluentSimplexObject::uvs() {
	return _uvs;
}

unsigned int *DfxFluentSimplexObject::faceIDs() {
	return fids;
}

void DfxFluentSimplexObject::setTriangleCount(unsigned int  cnt) {
	nbFaces = cnt;
}

void DfxFluentSimplexObject::setVertexCount(unsigned int  cnt) {
	nbVerts = cnt;
}

void DfxFluentSimplexObject::setEdgeCount(unsigned int cnt) {
	nbEdges = cnt;
}

void DfxFluentSimplexObject::setFaceIDs(unsigned int  *fid) {
	fids = fid;
}

void DfxFluentSimplexObject::setVertices(float *vts) {
	verts = vts;
}

void DfxFluentSimplexObject::setEdges(unsigned int *edgs) {
	_edges = edgs;
}

void DfxFluentSimplexObject::setSmoothNormals(float *arr) {
	vnormals = arr;
}

void DfxFluentSimplexObject::setFaceNormals(float *arr) {
	fnormals = arr;
}

void DfxFluentSimplexObject::setUvs(float *arr) {
	_uvs = arr;
}

void DfxFluentSimplexObject::calcNormals() {

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

void DfxFluentSimplexObject::clearVertices() {
	delete[] verts;
	verts = nullptr;
	nbVerts = 0;
}

void DfxFluentSimplexObject::clearIndices() {
	delete[] _indices;
	_indices = nullptr;
	nbFaces = 0;
}

void DfxFluentSimplexObject::clearSmoothNormals() {
	delete[] vnormals;
	vnormals = nullptr;
}

void DfxFluentSimplexObject::clearFaceNormals() {
	delete[] fnormals;
	fnormals = nullptr;
}

void DfxFluentSimplexObject::clearFaceIDs() {
	delete[] fids;
	fids = nullptr;
}

void DfxFluentSimplexObject::clearUvs() {
	delete[] _uvs;
	_uvs = nullptr;
}

void DfxFluentSimplexObject::clearAll() {
	clearVertices();
	clearIndices();
	clearSmoothNormals();
	clearFaceNormals();
	clearFaceIDs();
	clearUvs();
}

void DfxFluentSimplexObject::copyMesh(Mesh *mesh) {
	clearAll();

	if (mesh->nbVerts > 0) {

		verts = new float[mesh->nbVerts * 3];
		vnormals = new float[mesh->nbVerts * 3];
		nbVerts = mesh->nbVerts;

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;

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

	auto triCount = mesh->nbFaces;
	if (triCount > 0) {
		_indices = new unsigned int[mesh->nbFaces * 3];
		nbFaces = triCount;

		fnormals = new float[nbFaces * 3];
		fids = new unsigned int[nbFaces];
		_uvs = new float[nbFaces * 3 * 2];

		for (int i = 0; i < (mesh->nbFaces * 3); i++) {
			_indices[i] = mesh->indices[i];
		}

		for (int i = 0; i < nbFaces; i++) {
			int counter = i * 3;
			int uvCounter = i * 6;
			int uvCounter00 = i * 3;

			fnormals[counter] = 0.0f;
			fnormals[counter + 1] = 0.0f;
			fnormals[counter + 2] = 0.0f;

			_uvs[uvCounter] = mesh->uvs[uvCounter00].x;
			_uvs[uvCounter + 1] = mesh->uvs[uvCounter00].y;

			_uvs[uvCounter + 2] = mesh->uvs[uvCounter00 + 1].x;
			_uvs[uvCounter + 3] = mesh->uvs[uvCounter00 + 1].y;

			_uvs[uvCounter + 4] = mesh->uvs[uvCounter00 + 2].x;
			_uvs[uvCounter + 5] = mesh->uvs[uvCounter00 + 2].y;

			if (mesh->materialID != NULL) {
				fids[i] = mesh->materialID[i];
			}
			else {
				fids[i] = 0;
			}

		}
	}

	buildEdgeList();
}

void DfxFluentSimplexObject::copyMesh(deepsg::SgGeoAdapterInterface *adapter, glm::vec3 scale) {
	clearAll();

	if (adapter->getVertexCount() > 0) {
		verts = new float[adapter->getVertexCount() * 3];
		vnormals = new float[adapter->getVertexCount() * 3];
		nbVerts = adapter->getVertexCount();

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;

			auto v = adapter->getVertex(i) * scale;
			auto n = adapter->getSmoothNormal(i);

			verts[counter] = v.x;
			verts[counter + 1] = v.y;
			verts[counter + 2] = v.z;

			vnormals[counter] = n.x;
			vnormals[counter + 1] = n.y;
			vnormals[counter + 2] = n.z;

		}
	}

	auto triCount = adapter->getTriangleIndexListSize() / 3;
	if (triCount > 0) {
		_indices = adapter->getTriangleIndexList();
		std::unique_ptr<unsigned int> sourceFaceIndices(adapter->getTriangleSourceFaceIndexList());
		nbFaces = triCount;

		fnormals = new float[nbFaces * 3];
		fids = new unsigned int[nbFaces];
		_uvs = new float[nbFaces * 3 * 2];

		for (int i = 0; i < nbFaces; i++) {
			int counter = i * 3;
			int uvCounter = i * 6;

			fnormals[counter] = 0.0f;
			fnormals[counter + 1] = 0.0f;
			fnormals[counter + 2] = 0.0f;

			auto uvMap = adapter->getPolygonUVMap(sourceFaceIndices.get()[i]);

			auto auv = uvMap[_indices[counter]];
			auto buv = uvMap[_indices[counter + 1]];
			auto cuv = uvMap[_indices[counter + 2]];

			_uvs[uvCounter] = auv.x;
			_uvs[uvCounter + 1] = auv.y;

			_uvs[uvCounter + 2] = buv.x;
			_uvs[uvCounter + 3] = buv.y;

			_uvs[uvCounter + 4] = cuv.x;
			_uvs[uvCounter + 5] = cuv.y;

			fids[i] = 0;
		}
	}

	buildEdgeList();
}

void DfxFluentSimplexObject::copyGeometryNode(deepsg::SgGeoNode *node) {
	clearAll();

	std::unordered_map<unsigned int, int> materialID;
	auto adapter = node->getGeometryAdapter();

	auto matLinks = DfxMaterialLinker::getInstance()->allObjectLinks(node->referencePath());
	
	for (auto link : matLinks) {
		auto matAny = DfxSpace::getInstance()->findAbsolute(link.second);

		if (matAny != nullptr) {
			if (matAny->type() == fl::type::DATA_OBJECT) {
				auto matDao = matAny->rawData<fl::DataObject>();
		
				materialID[link.first] = matDao->property("material_id")->data<int>();
			}
		}
	}

	if (adapter->getVertexCount() > 0) {
		verts = new float[adapter->getVertexCount() * 3];
		vnormals = new float[adapter->getVertexCount() * 3];
		nbVerts = adapter->getVertexCount();

		for (int i = 0; i < nbVerts; i++) {
			int counter = i * 3;

			auto v = adapter->getVertex(i);
			auto n = adapter->getSmoothNormal(i);

			verts[counter] = v.x;
			verts[counter + 1] = v.y;
			verts[counter + 2] = v.z;

			vnormals[counter] = n.x;
			vnormals[counter + 1] = n.y;
			vnormals[counter + 2] = n.z;

		}
	}

	auto triCount = adapter->getTriangleIndexListSize() / 3;
	if (triCount > 0) {

		_indices = adapter->getTriangleIndexList();
		std::unique_ptr<unsigned int> sourceFaceIndices(adapter->getTriangleSourceFaceIndexList());
		nbFaces = triCount;

		fnormals = new float[nbFaces * 3];
		fids = new unsigned int[nbFaces];
		_uvs = new float[nbFaces * 3 * 2];

		for (int i = 0; i < nbFaces; i++) {
			int counter = i * 3;
			int uvCounter = i * 6;

			fnormals[counter] = 0.0f;
			fnormals[counter + 1] = 0.0f;
			fnormals[counter + 2] = 0.0f;

			auto uvMap = adapter->getPolygonUVMap(sourceFaceIndices.get()[i]);

			auto auv = uvMap[_indices[counter]];
			auto buv = uvMap[_indices[counter + 1]];
			auto cuv = uvMap[_indices[counter + 2]];

			_uvs[uvCounter] = auv.x;
			_uvs[uvCounter + 1] = auv.y;

			_uvs[uvCounter + 2] = buv.x;
			_uvs[uvCounter + 3] = buv.y;

			_uvs[uvCounter + 4] = cuv.x;
			_uvs[uvCounter + 5] = cuv.y;

			auto itrMat = materialID.find(adapter->getPolygonSurfaceID(sourceFaceIndices.get()[i]));
			
			if (itrMat != materialID.end())
				fids[i] = itrMat->second;
			else
				fids[i] = 0;
		}
	}

	buildEdgeList();
}

deepsg::SgGeoAdapterInterface *DfxFluentSimplexObject::getGeometryAdapter() {
	return _interface;
}

DfxFluentChannelMap &DfxFluentSimplexObject::vertexAttributes() {
	return _vertexAttributes;
}

DfxFluentChannelMap &DfxFluentSimplexObject::faceAttributes() {
	return _faceAttributes;
}

glm::mat4 DfxFluentSimplexObject::moveToOrigin() {
	DfxBounds bounds;

	for (size_t inx = 0; inx < vertexCount(); inx++) {
		size_t offset = inx * 3;

		glm::vec3 p = glm::vec3(vertices()[offset], vertices()[offset + 1], vertices()[offset + 2]);
		bounds.add(p.x, p.y, p.z);
	}

	glm::vec3 pos = bounds.center();

	for (size_t inx = 0; inx < vertexCount(); inx++) {
		size_t offset = inx * 3;
		glm::vec3 p = glm::vec3(vertices()[offset], vertices()[offset + 1], vertices()[offset + 2]);
		glm::vec3 newp = p - pos;

		vertices()[offset] = newp.x;
		vertices()[offset + 1] = newp.y;
		vertices()[offset + 2] = newp.z;
	}


	glm::mat4 transMat = glm::translate(glm::mat4(1.f), pos);

	transMat *= glm::scale(glm::mat4(1.f), glm::vec3(1.f));
	
	return transMat;
}

glm::vec3 DfxFluentSimplexObject::center() {
	glm::vec3 output(0.f);

	rtx::AABB bounds;
	bounds.empty();
	if (nbVerts > 0) {
		for (size_t inx = 0; inx < vertexCount(); inx++) {
			auto offset = inx * 3;
			bounds.add(vertices()[offset], vertices()[offset + 1], vertices()[offset + 2]);
		}

		float center[3];
		bounds.center2(center);
		output = glm::vec3(center[0], center[1], center[2]);
	}

	return output;
}


uint32_t DfxFluentSimplexObject::vertexVersion() {
	return _vertexVersion;
}

uint32_t DfxFluentSimplexObject::incrementVertexVersion() {
	return ++_vertexVersion;
}

uint32_t DfxFluentSimplexObject::uvVersion() {
	return _uvVersion;
}

uint32_t DfxFluentSimplexObject::incrementUvVersion() {
	return ++_uvVersion;
}

uint32_t DfxFluentSimplexObject::vertexNormalVersion() {
	return _vertexNormalVersion;
}

uint32_t DfxFluentSimplexObject::incrementVertexNormalVersion() {
	return ++_vertexNormalVersion;
}

uint32_t DfxFluentSimplexObject::materialIDVersion() {
	return _materialIDVersion;
}

uint32_t DfxFluentSimplexObject::incrementMaterialIDVersion() {
	return ++_materialIDVersion;
}

uint32_t DfxFluentSimplexObject::indexVersion() {
	return _indexVersion;
}

uint32_t DfxFluentSimplexObject::incrementIndexVersion() {
	return ++_indexVersion;
}

uint32_t DfxFluentSimplexObject::topolgyVersion() {
	return _topolgyVersion;
}

uint32_t DfxFluentSimplexObject::incrementTopolgyVersion() {
	return ++_topolgyVersion;
}