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

#include "SgGeoAdapterInterface.h"

namespace deepsg {
	class SgDynaMesh {
	public:

		virtual unsigned int  triangleCount() = 0;
		virtual unsigned int  vertexCount() = 0;
		virtual float *vertices() = 0;
		virtual unsigned int  *indices() = 0;
		virtual unsigned int *edges() = 0;
		virtual float *smoothNormals() = 0;
		virtual float *faceNormals() = 0;
		virtual unsigned int *faceIDs() = 0;
		virtual float *uvs() = 0;

		virtual void setTriangleCount(unsigned int) = 0;
		virtual void setVertexCount(unsigned int) = 0;
		virtual void setEdgeCount(unsigned int) = 0;
		virtual unsigned int edgeCount() = 0;

		virtual void setVertices(float *) = 0;
		virtual void setIndices(unsigned int  *) = 0;
		virtual void setEdges(unsigned int  *) = 0;
		virtual void setSmoothNormals(float *) = 0;
		virtual void setFaceNormals(float *) = 0;
		virtual void setUvs(float *) = 0;
		virtual void setFaceIDs(unsigned int  *) = 0;

		virtual void calcNormals() = 0;

		virtual void clearVertices() = 0;
		virtual void clearIndices() = 0;
		virtual void clearSmoothNormals() = 0;
		virtual void clearFaceNormals() = 0;
		virtual void clearFaceIDs() = 0;
		virtual void clearUvs() = 0;

		virtual void clearAll() = 0;

		virtual SgGeoAdapterInterface *getGeometryAdapter() = 0;
	};
}