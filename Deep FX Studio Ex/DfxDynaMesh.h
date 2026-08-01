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

#include <SgGeoNode.h>
#include <Mesh.h>
#include "SgDynaMesh.h"
#include "DfxObjectCache.h"

class DfxDynaMesh : public deepsg::SgDynaMesh {
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

	unsigned int nbEdges;
	unsigned int nbVerts;
	unsigned int nbFaces;

public:

	DfxDynaMesh();
	virtual ~DfxDynaMesh();

	unsigned int triangleCount() override;
	unsigned int vertexCount() override;
	unsigned int edgeCount() override;

	float *vertices() override;
	unsigned int *indices() override;
	unsigned int *edges() override;
	float *smoothNormals() override;
	float *faceNormals() override;
	unsigned int *faceIDs() override;
	float *uvs() override;

	void setTriangleCount(unsigned int) override;
	void setVertexCount(unsigned int) override;
	void setEdgeCount(unsigned int) override;

	void setVertices(float *) override;
	void setIndices(unsigned int  *) override;
	void setEdges(unsigned int  *) override;
	void setSmoothNormals(float *) override;
	void setFaceNormals(float *) override;
	void setFaceIDs(unsigned int  *) override;
	void setUvs(float *) override;

	void buildEdgeList();
	void calcNormals() override;

	void clearVertices() override;
	void clearIndices() override;
	void clearSmoothNormals() override;
	void clearFaceNormals() override;
	void clearFaceIDs() override;
	void clearUvs() override;

	void copyMesh(deepsg::SgGeoNode *);
	void copyMesh(Mesh *, std::string objPath = "");

	DfxObjectCache *cache();

	deepsg::SgGeoAdapterInterface *getGeometryAdapter() override;

	void clearAll() override;
};