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

#include "DfxObjectCache.h"
#include "DfxDeepSimplexGeoAdapter.h"

#define BUFFER_OFFSET(i) ((void*)(i))

size_t DfxObjectCache::ID_GEN = 100;

DfxObjectCache::DfxObjectCache() {

	_id = ++ID_GEN;

	_any = nullptr;

	_vertex_buffer_update = nullptr;
	_vertex_buffer_update_size = 0;

	_interleavedPointsVBO_active	= false;
	_triangleIndicesVBO_active		= false;
	_lineIndicesVBO_active			= false;
	_pointIndicesVBO_active			= false;
	_selectionPointsVBO_active		= false;

	selectionPointsVBO = 0;
	interleavedPointsVBO = 0;
	triangleIndicesVBO = 0;
	lineIndicesVBO = 0;
	pointIndicesVBO = 0;

	_isCached = false;
}

DfxObjectCache::~DfxObjectCache() {
	clear();
}

size_t DfxObjectCache::id() {
	return _id;
}

void DfxObjectCache::setVertexBufferUpdate(float *buffer, size_t buffer_size) {
	_vertex_buffer_update = buffer;
	_vertex_buffer_update_size = buffer_size;
}


deepsg::SgGeoAdapterInterface *DfxObjectCache::meshData() {
	return _meshData;
}

void DfxObjectCache::setMeshData(deepsg::SgGeoAdapterInterface *geo) {
	_meshData = geo;
}

fl::Any *DfxObjectCache::any() {
	return _any;
}

void DfxObjectCache::setAny(fl::Any *ay) {
	_any = ay;
}

bool DfxObjectCache::isCached() {
	return _isCached;
}

void DfxObjectCache::createSelectionPointsVBO() {
	if (!_selectionPointsVBO_active) {
		glGenBuffers(1, &selectionPointsVBO);
		_selectionPointsVBO_active = true;
	}
}

void DfxObjectCache::createInterleavedPointsVBO() {
	if (!_interleavedPointsVBO_active) {
		glGenBuffers(1, &interleavedPointsVBO);
		_interleavedPointsVBO_active = true;
	}
}

void DfxObjectCache::createTriangleIndicesVBO() {
	if (!_triangleIndicesVBO_active) {
		glGenBuffers(1, &triangleIndicesVBO);
		_triangleIndicesVBO_active = true;
	}
}

void DfxObjectCache::createLineIndicesVBO() {
	if (!_lineIndicesVBO_active) {
		glGenBuffers(1, &lineIndicesVBO);
		_lineIndicesVBO_active = true;
	}
}

void DfxObjectCache::createPointIndicesVBO() {
	if (!_pointIndicesVBO_active) {
		glGenBuffers(1, &pointIndicesVBO);
		_pointIndicesVBO_active = true;
	}
}

void DfxObjectCache::clearInterleavedPointsVBO() {
	if (_interleavedPointsVBO_active) {
		glDeleteBuffers(1, &interleavedPointsVBO);
		_interleavedPointsVBO_active = false;
		interleavedPointsVBO = 0;
	}
}

void DfxObjectCache::clearTriangleIndicesVBO() {
	if (_triangleIndicesVBO_active) {
		glDeleteBuffers(1, &triangleIndicesVBO);
		_triangleIndicesVBO_active = false;
		triangleIndicesVBO = 0;
	}
}

void DfxObjectCache::clearLineIndicesVBO() {
	if (_lineIndicesVBO_active) {
		glDeleteBuffers(1, &lineIndicesVBO);
		_lineIndicesVBO_active = false;
		lineIndicesVBO = 0;
	}
}

void DfxObjectCache::clearPointIndicesVBO() {
	if (_pointIndicesVBO_active) {
		glDeleteBuffers(1, &pointIndicesVBO);
		_pointIndicesVBO_active = false;
		pointIndicesVBO = 0;
	}
}

void DfxObjectCache::clearSelectionPointsVBO() {
	if (_selectionPointsVBO_active) {
		glDeleteBuffers(1, &selectionPointsVBO);
		_selectionPointsVBO_active = false;
		selectionPointsVBO = 0;
	}
}

void DfxObjectCache::clear() {
	clearInterleavedPointsVBO();
	clearTriangleIndicesVBO();
	clearLineIndicesVBO();
	clearPointIndicesVBO();
	clearSelectionPointsVBO();
}

GLfloat *DfxObjectCache::_generateSelectionPointList(float r, float g, float b) {
	GLfloat *varray = new GLfloat[_meshData->getVertexCount() * 3 * 6];

	size_t counter = 0;
	for (size_t inx = 0; inx < _meshData->getVertexCount(); inx++) {
	
		varray[counter]     = _meshData->getVertex(inx).x;
		varray[counter + 1] = _meshData->getVertex(inx).y;
		varray[counter + 2] = _meshData->getVertex(inx).z;
		
		varray[counter + 3] = r;
		varray[counter + 4] = g;
		varray[counter + 5] = b;

		counter += 6;
	}

	return varray;
}

void DfxObjectCache::cacheInterleavedPointsVBO(float *vertexBuffer) {
	bool rc = initializeOpenGLFunctions();
	
	clearInterleavedPointsVBO();

	bool smooth_shading = true;

	if (_any != nullptr) {
		if (_any->type() == fl::type::DATA_OBJECT) {
			if (_any->rawData<fl::DataObject>()->hasProperty("smooth_shading")) {
				auto prop = _any->rawData<fl::DataObject>()->property("smooth_shading");

				if (prop->type() == fl::type::BOOL) smooth_shading = prop->data<bool>();
			}
		}
	}
	
	createInterleavedPointsVBO();

	std::unique_ptr<GLfloat[]> vertices;

	if (smooth_shading) {

		if(_vertex_buffer_update != nullptr)
			vertices.reset(_meshData->getInterleavedVertexList(_vertex_buffer_update));
		else 
			vertices.reset(_meshData->getInterleavedVertexList(vertexBuffer));

		glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _meshData->getVertexCount() * 8, &vertices.get()[0], GL_STATIC_DRAW);
	}
	else {
		int triangleIndexCount = _meshData->getTriangleIndexListSize();

		if (_vertex_buffer_update != nullptr)
			vertices.reset(_meshData->getInterleavedVertexList(_vertex_buffer_update));
		else
			vertices.reset(_meshData->getInterleavedVertexList(vertexBuffer));

		glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangleIndexCount * 8 * 3, &vertices.get()[0], GL_STATIC_DRAW);
	}
	
}

void DfxObjectCache::cacheLineIndicesVBO() {
	initializeOpenGLFunctions();
	clearLineIndicesVBO();

	createLineIndicesVBO();

	int edgeIndexCount = _meshData->getEdgeIndexListSize();

	if (edgeIndexCount > 0) {
		std::unique_ptr<GLuint[]> indices;

		indices.reset(_meshData->getEdgeIndexList());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndicesVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*edgeIndexCount, &indices.get()[0], GL_STATIC_DRAW);
	}
	
}

void DfxObjectCache::cacheTriangleIndicesVBO() {
	bool rc = initializeOpenGLFunctions();
	clearTriangleIndicesVBO();

	bool smooth_shading = true;

	if (_any != nullptr) {
		if (_any->type() == fl::type::DATA_OBJECT) {
			if (_any->rawData<fl::DataObject>()->hasProperty("smooth_shading")) {
				auto prop = _any->rawData<fl::DataObject>()->property("smooth_shading");

				if (prop->type() == fl::type::BOOL) smooth_shading = prop->data<bool>();
			}
		}
	}

	createTriangleIndicesVBO();

	std::unique_ptr<GLuint[]> indices;
	int triangleIndexCount = _meshData->getTriangleIndexListSize();

	if (smooth_shading) {
		indices.reset(_meshData->getTriangleIndexList());
	}
	else {
		indices.reset(_meshData->getFlatTriangleIndexList());
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*triangleIndexCount, &indices.get()[0], GL_STATIC_DRAW);

}

void DfxObjectCache::cacheMesh() {

	cacheInterleavedPointsVBO();
	cacheLineIndicesVBO();
	cacheTriangleIndicesVBO();

	_isCached = true;
}

void DfxObjectCache::drawMesh(DfxQViewportRenderInterface::ShadeMode smode, float lineWidth, std::vector<glm::mat4> xforms, bool instanced) {
	initializeOpenGLFunctions();

	if (smode == DfxQViewportRenderInterface::ShadeMode::WIREFRAME) {

		if (_interleavedPointsVBO_active && _triangleIndicesVBO_active) {
			int triangleIndexCount = _meshData->getTriangleIndexListSize();
			int edgeIndexCount = _meshData->getEdgeIndexListSize();

			enableWireframeState();

			glLineWidth(lineWidth);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
			glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 3));
			glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 6));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndicesVBO);

			if (instanced) {
				for (auto xform : xforms) {
					glPushMatrix();
					glMultMatrixf(&xform[0][0]);
					glDrawElements(GL_LINES, edgeIndexCount, GL_UNSIGNED_INT, 0);
					glPopMatrix();
				}
			}
			else {
				glDrawElements(GL_LINES, edgeIndexCount, GL_UNSIGNED_INT, 0);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			
			glLineWidth(1.f);

			disableWireframeState();
		}

	}
	else if (smode == DfxQViewportRenderInterface::ShadeMode::WIREFRAME_SOLID_SHADE) {

		if (_interleavedPointsVBO_active && _triangleIndicesVBO_active) {

			int triangleIndexCount = _meshData->getTriangleIndexListSize();
			int edgeIndexCount = _meshData->getEdgeIndexListSize();

			enableSolidShadeState();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
			glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 3));
			glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 6));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndicesVBO);

			glPolygonOffset(0.0f, 5.0f);

			if (instanced) {
				for (auto xform : xforms) {
					glPushMatrix();
					glMultMatrixf(&xform[0][0]);
					glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);
					glPopMatrix();
				}
			}
			else {
				glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			disableSolidShadeState();

			enableWireframeState();

			glLineWidth(lineWidth);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
			glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 3));
			glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 6));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndicesVBO);

			if (instanced) {
				for (auto xform : xforms) {
					glPushMatrix();
					glMultMatrixf(&xform[0][0]);
					glDrawElements(GL_LINES, edgeIndexCount, GL_UNSIGNED_INT, 0);
					glPopMatrix();
				}
			}
			else {
				glDrawElements(GL_LINES, edgeIndexCount, GL_UNSIGNED_INT, 0);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glLineWidth(1.f);

			disableWireframeState();
		}
	}
	else if (smode == DfxQViewportRenderInterface::ShadeMode::SOLID_SHADE) {

		if (_interleavedPointsVBO_active && _triangleIndicesVBO_active) {
			int triangleIndexCount = _meshData->getTriangleIndexListSize();

			enableSolidShadeState();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
			glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 3));
			glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 6));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndicesVBO);

			if (instanced) {
				for (auto xform : xforms) {
					glPushMatrix();
					glMultMatrixf(&xform[0][0]);
					glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);
					glPopMatrix();
				}
			}
			else {
				glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			disableSolidShadeState();
		}
	}
}

void DfxObjectCache::drawMeshElements(std::vector<unsigned int > indices) {
	if (_interleavedPointsVBO_active && _triangleIndicesVBO_active) {
		
		int triangleIndexCount = indices.size();

		if (triangleIndexCount > 0) {

			glDisable(GL_LIGHTING);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
			glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 3));
			glTexCoordPointer(2, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(sizeof(GLfloat) * 6));

			glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, &indices[0]);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
		}
	}

}

void DfxObjectCache::drawDiffuse(int r, int g, int b) {
	GLuint colorVBO;
	glGenBuffers(1, &colorVBO);

	std::unique_ptr<GLfloat[]> colors(new GLfloat[_meshData->getVertexCount() * 3]);

	for (size_t inx = 0; inx < _meshData->getVertexCount(); inx++) {
		size_t current = 3 * inx;
		colors.get()[current]	  = (GLfloat)r / 255.f;
		colors.get()[current + 1] = (GLfloat)g / 255.f;
		colors.get()[current + 2] = (GLfloat)b / 255.f;
	}

	int triangleIndexCount = _meshData->getTriangleIndexListSize();

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, interleavedPointsVBO);
	glVertexPointer(3, GL_FLOAT, 8 * sizeof(float), BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _meshData->getVertexCount() * 3, &colors.get()[0], GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndicesVBO);
	glDrawElements(GL_TRIANGLES, triangleIndexCount, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDeleteBuffers(1, &pointIndicesVBO);
}

void DfxObjectCache::enableSolidShadeState() {

	glEnable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}

void DfxObjectCache::disableSolidShadeState() {
	glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHT1);

	glDisable(GL_NORMALIZE);

	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);

	glDisable(GL_AUTO_NORMAL);
}

void DfxObjectCache::enableWireframeState() {
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.f);
}

void DfxObjectCache::disableWireframeState() {
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.f);
}