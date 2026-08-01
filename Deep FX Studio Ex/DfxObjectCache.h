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

#include <QObject>
#include <SgGeoAdapterInterface.h>
#include <QOpenGlFunctions_3_1>
#include <QOpenGLWidget>
#include <FlAny.h>
#include <DfxSpace.h>
#include <QOpenGLWidget.h>
#include "DfxQViewportRenderInterface.h"
#include "DfxMode.h"

class DfxObjectCache: public QObject, public QOpenGLFunctions_3_1 {
	Q_OBJECT
private:
	static size_t ID_GEN;
	size_t _id;

	deepsg::SgGeoAdapterInterface *_meshData;
	fl::Any *_any;
private:

	bool _selectionPointsVBO_active;
	GLuint selectionPointsVBO;

	bool _interleavedPointsVBO_active;
	GLuint interleavedPointsVBO;

	bool _triangleIndicesVBO_active;
	GLuint triangleIndicesVBO;

	bool _lineIndicesVBO_active;
	GLuint lineIndicesVBO;

	bool _pointIndicesVBO_active;
	GLuint pointIndicesVBO;

	GLfloat *_generateSelectionPointList(float r, float g, float b);

	bool _isCached;

private:
	float *_vertex_buffer_update;
	size_t _vertex_buffer_update_size;
public:

	DfxObjectCache();
	virtual ~DfxObjectCache();

	size_t id();

	deepsg::SgGeoAdapterInterface *meshData();
	void setMeshData(deepsg::SgGeoAdapterInterface *);

	fl::Any *any();
	void setAny(fl::Any *ay);

	void cacheInterleavedPointsVBO(float *vertexBuffer=nullptr);
	void cacheLineIndicesVBO();
	void cacheTriangleIndicesVBO();

	void createSelectionPointsVBO();
	void createInterleavedPointsVBO();
	void createTriangleIndicesVBO();
	void createLineIndicesVBO();
	void createPointIndicesVBO();

	void clearSelectionPointsVBO();
	void clearInterleavedPointsVBO();
	void clearTriangleIndicesVBO();
	void clearLineIndicesVBO();
	void clearPointIndicesVBO();

	void clear();

	void drawMesh(DfxQViewportRenderInterface::ShadeMode smode, float lineWidth = 1.f, std::vector<glm::mat4> xforms = std::vector<glm::mat4>(), bool instanced = false);
	void drawMeshElements(std::vector<unsigned int > indices);

	void drawPoints(std::vector<glm::ivec3> colors);
	void drawDiffuse(int r, int g, int b);

	void enableSolidShadeState();
	void disableSolidShadeState();

	void enableWireframeState();
	void disableWireframeState();

	bool isCached();

	void setVertexBufferUpdate(float *buffer, size_t buffer_size);

public slots:
	void cacheMesh();
};