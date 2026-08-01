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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <RtxAABB.h>
#include <SgGeoNode.h>
#include <DfxMaterialLinker.h>
#include "Mesh.h"
#include "DfxObjectCache.h"
#include "DfxFluentChannelMap.h"

class DfxFluentSimplexObject {
private:

	deepsg::SgGeoAdapterInterface *_interface;
	DfxObjectCache *_cache;

	float *verts;
	float *vnormals;
	float *fnormals;
	unsigned int *_edges;
	unsigned int *_indices;
	unsigned int *fids;
	float *_uvs;

	uint32_t _vertexVersion;
	uint32_t _uvVersion;
	uint32_t _vertexNormalVersion;
	uint32_t _materialIDVersion;
	uint32_t _indexVersion;
	uint32_t _topolgyVersion;

	unsigned int nbEdges;
	unsigned int nbVerts;
	unsigned int nbFaces;

	DfxFluentChannelMap _vertexAttributes;
	DfxFluentChannelMap _faceAttributes;

	
public:

	DfxFluentSimplexObject();
	virtual ~DfxFluentSimplexObject();

	unsigned int triangleCount();
	unsigned int vertexCount();
	unsigned int edgeCount();

	float *vertices();
	unsigned int *indices();
	unsigned int *edges();
	float *smoothNormals();
	float *faceNormals();
	unsigned int *faceIDs();
	float *uvs();

	void setTriangleCount(unsigned int);
	void setVertexCount(unsigned int);
	void setEdgeCount(unsigned int);

	void setVertices(float *);
	void setIndices(unsigned int  *);
	void setEdges(unsigned int  *);
	void setSmoothNormals(float *);
	void setFaceNormals(float *);
	void setFaceIDs(unsigned int  *);
	void setUvs(float *);

	void buildEdgeList();
	void calcNormals();

	void clearVertices();
	void clearIndices();
	void clearSmoothNormals();
	void clearFaceNormals();
	void clearFaceIDs();
	void clearUvs();

	glm::mat4 moveToOrigin();

	DfxFluentChannelMap &vertexAttributes();
	DfxFluentChannelMap &faceAttributes();

	void copyMesh(Mesh *);
	void copyMesh(deepsg::SgGeoAdapterInterface *, glm::vec3 scale = glm::vec3(1.f));
	void copyGeometryNode(deepsg::SgGeoNode *node);

	DfxObjectCache *cache();

	deepsg::SgGeoAdapterInterface *getGeometryAdapter();

	void clearAll();

	glm::vec3 center();

	uint32_t vertexVersion();
	uint32_t incrementVertexVersion();

	uint32_t uvVersion();
	uint32_t incrementUvVersion();

	uint32_t vertexNormalVersion();
	uint32_t incrementVertexNormalVersion();

	uint32_t materialIDVersion();
	uint32_t incrementMaterialIDVersion();

	uint32_t indexVersion();
	uint32_t incrementIndexVersion();

	uint32_t topolgyVersion();
	uint32_t incrementTopolgyVersion();
};