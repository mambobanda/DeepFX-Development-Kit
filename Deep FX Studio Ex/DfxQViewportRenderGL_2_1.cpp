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

#include <DsTools.h>
#include <DsMeshOperator.h>

#include "DfxQViewportRenderGL_2_1.h"
#include "DfxGLSLShaderWrangler.h"
#include "DfxPointCloudHandler.h"
#include "DfxDynaMesh.h"

bool isCached = false;
GLuint cacheID = 0;

#define BUFFER_OFFSET(i) ((void*)(i))

DfxQViewportRenderGL_2_1::DfxQViewportRenderGL_2_1() {
	widgetRatio = 0.011;
	_shadingMode = ShadeMode::SOLID_SHADE;
}

DfxQViewportRenderGL_2_1::~DfxQViewportRenderGL_2_1() {

}

DfxQViewportRenderInterface::ShadeMode DfxQViewportRenderGL_2_1::shadingMode() {
	return _shadingMode;
}

void DfxQViewportRenderGL_2_1::setShadingMode(DfxQViewportRenderInterface::ShadeMode sm) {
	_shadingMode = sm;
}

void DfxQViewportRenderGL_2_1::setCameraTransform(glm::mat4 cform) {
	cameraTransform = cform;
}

void DfxQViewportRenderGL_2_1::applyShadedSolidState() {

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_POINT_SMOOTH);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);

	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);

}

void DfxQViewportRenderGL_2_1::undoShadedSolidState() {

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

void DfxQViewportRenderGL_2_1::applyGridDrawingState() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}

void DfxQViewportRenderGL_2_1::undoGridDrawingState() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
}

void DfxQViewportRenderGL_2_1::applySelectedObjectSolidState() {	
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(3.f);
}

void DfxQViewportRenderGL_2_1::undoSelectedObjectSolidState() {
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.f);
}

void DfxQViewportRenderGL_2_1::applyWireSolidState() {
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.5f);
}

void DfxQViewportRenderGL_2_1::undoWireSolidState() {
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.f);
}

void DfxQViewportRenderGL_2_1::applyVertexState() {
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.50f);
}

void DfxQViewportRenderGL_2_1::undoVertexState() {
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1.f);
}

void DfxQViewportRenderGL_2_1::applySelectedEdgeState() {
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.5f);
}

void DfxQViewportRenderGL_2_1::undoSelectedEdgeState() {
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1.f);
}

void DfxQViewportRenderGL_2_1::applyPointCloudPointsState() {
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(3.5f);
}

void DfxQViewportRenderGL_2_1::undoPointCloudPointsState() {
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPointSize(1.f);
}

void DfxQViewportRenderGL_2_1::applySolidOutlineState() {
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void DfxQViewportRenderGL_2_1::undoSolidOutlineState() {
	glDisable(GL_STENCIL_TEST);
}

std::string DfxQViewportRenderGL_2_1::name() {
	return "Viewport Render Gl 2.1 v1.0";
}

void DfxQViewportRenderGL_2_1::initLighting() {
	float lightDirection[3] = { -0.57735f, -0.57735f, -0.57735f };
	float v[4];

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	v[0] = v[1] = v[2] = 1.f*0.3f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = 1.1*0.9f; v[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
	v[0] = -lightDirection[0]; v[1] = -lightDirection[1]; v[2] = -lightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, v);

	v[0] = v[1] = v[2] = 1.f*0.2f; v[3] = 1.0f;
	glLightfv(GL_LIGHT1, GL_AMBIENT, v);
	v[0] = v[1] = v[2] = 1.5*0.4f; v[3] = 1.0f;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
	v[0] = lightDirection[0]; v[1] = lightDirection[1]; v[2] = lightDirection[2]; v[3] = 0.0f;
	glLightfv(GL_LIGHT1, GL_POSITION, v);
}

void DfxQViewportRenderGL_2_1::init() {
	initializeOpenGLFunctions();
	initLighting();
}

void drawCube(float s) {

	glBegin(GL_TRIANGLES);

	glNormal3f(0.f, 0.f, -1.f);
	glVertex3f(-(s), (s), (s));
	glVertex3f((s), -(s), (s));
	glVertex3f(-(s), -(s), (s));

	glVertex3f((s), -(s), (s));
	glVertex3f(-(s), (s), (s));
	glVertex3f((s), (s), (s));

	glNormal3f(0.f, 0.f, 1.f);
	glVertex3f(-(s), -(s), -(s));
	glVertex3f((s), -(s), -(s));
	glVertex3f(-(s), (s), -(s));

	glVertex3f((s), (s), -(s));
	glVertex3f(-(s), (s), -(s));
	glVertex3f((s), -(s), -(s));

	glNormal3f(0.f, -1.f, 1.f);
	glVertex3f((s), -(s), -(s));
	glVertex3f((s), -(s), (s));
	glVertex3f((s), (s), -(s));

	glVertex3f((s), (s), (s));
	glVertex3f((s), (s), -(s));
	glVertex3f((s), -(s), (s));

	glNormal3f(0.f, 1.f, 1.f);
	glVertex3f(-(s), (s), -(s));
	glVertex3f(-(s), -(s), (s));
	glVertex3f(-(s), -(s), -(s));

	glVertex3f(-(s), -(s), (s));
	glVertex3f(-(s), (s), -(s));
	glVertex3f(-(s), (s), (s));

	glNormal3f(-1.f, 0.f, 0.f);
	glVertex3f((s), (s), -(s));
	glVertex3f(-(s), (s), (s));
	glVertex3f(-(s), (s), -(s));

	glVertex3f(-(s), (s), (s));
	glVertex3f((s), (s), -(s));
	glVertex3f((s), (s), (s));

	glNormal3f(1.f, 0.f, 0.f);
	glVertex3f(-(s), -(s), -(s));
	glVertex3f(-(s), -(s), (s));
	glVertex3f((s), -(s), -(s));

	glVertex3f((s), -(s), (s));
	glVertex3f((s), -(s), -(s));
	glVertex3f(-(s), -(s), (s));

	glEnd();
}

void DfxQViewportRenderGL_2_1::render() {
	initializeOpenGLFunctions();

	//glEnable(GL_DEPTH_TEST);
	glClearColor(getBackgroundColor().x, 
				 getBackgroundColor().y, 
				 getBackgroundColor().z, 
				 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Apply Projection And View Matrix
	glm::mat4 pMatrix = getProjectionMatrix();
	glm::mat4 vMatrix = getViewMatrix();

	// Apply Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&pMatrix[0][0]);

	// Apply View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&vMatrix[0][0]);

	applyGridDrawingState();
	drawWorkGrid();
	undoGridDrawingState();

	renderFluentSimplexCacheTrackScene();
	renderFluentStreamCacheTrackScene();
	renderFluentSimplexScene();
	renderFluentStreamScene();
	renderScene();
	
}

void DfxQViewportRenderGL_2_1::renderColors(ElementType et) {

	glClearColor(getBackgroundColor().x,
				 getBackgroundColor().y,
				 getBackgroundColor().z,
				 1.0f);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Apply Projection And View Matrix
	glm::mat4 pMatrix = getProjectionMatrix();
	glm::mat4 vMatrix = getViewMatrix();

	// Apply Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&pMatrix[0][0]);

	// Apply View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&vMatrix[0][0]);

	if (et == ElementType::OBJECT) {
		deepsg::SgTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			renderGeoColorOnly(node, info);
		};

		deepsg::SgRootNode::getInstance()->traverseScene(renderFunc);

	}
	else if (et == ElementType::OBJECT_SHAPE) {
		deepsg::SgTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			renderGeoColorOnly(node, info);
			renderShapeColorOnly(node, info);
		};

		deepsg::SgRootNode::getInstance()->traverseScene(renderFunc);
	}
	else if (et == ElementType::VERTEX) {
		DfxColorField color(1);

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

			auto geo = DfxMeshSelectionModel::getInstance()->nodeReference()->getGeometryAdapter();
		
			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());

			glPushMatrix();

			glMultMatrixf(&mat[0][0]);

			DfxObjectHandler handler;
			if (handler.fetch(DfxMeshSelectionModel::getInstance()->nodeReference()->referencePath())) {
				auto cache = handler.objectCache();

				if (cache != nullptr) {
					cache->drawDiffuse(0,0,0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}

			glPointSize(8.f);
			glBegin(GL_POINTS);
			for (int i = 0; i < geo->getVertexCount(); i++) {
				glColor3ub(color.r(), color.g(), color.b());
				color.increment();

				
				glVertex3f(geo->getVertex(i).x,
					geo->getVertex(i).y,
					geo->getVertex(i).z);
			}
			glEnd();
			glPopMatrix();
		}
	}
	else if (et == ElementType::EDGE) {
		DfxColorField color(1);

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

			auto geo = DfxMeshSelectionModel::getInstance()->nodeReference()->getGeometryAdapter();

			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());

			glPushMatrix();

			glMultMatrixf(&mat[0][0]);

			DfxObjectHandler handler;
			if (handler.fetch(DfxMeshSelectionModel::getInstance()->nodeReference()->referencePath())) {
				auto cache = handler.objectCache();

				if (cache != nullptr) {
					cache->drawDiffuse(0, 0, 0);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}

			glLineWidth(4.f);
			glBegin(GL_LINES);
			for (int i = 0; i < geo->getEdgeCount(); i++) {
				glColor3ub(color.r(), color.g(), color.b());
				color.increment();

				auto eindices = geo->getEdgeVerticeIndices(i);

				glVertex3f(geo->getVertex(eindices.first).x,
						   geo->getVertex(eindices.first).y,
						   geo->getVertex(eindices.first).z);

				glVertex3f(geo->getVertex(eindices.second).x,
						   geo->getVertex(eindices.second).y,
						   geo->getVertex(eindices.second).z);
			}
			glEnd();
			glLineWidth(1.f);
			glPopMatrix();
		}
	}
	else if (et == ElementType::FACE) {
		DfxColorField color(1);

		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr) {

			auto geo = DfxMeshSelectionModel::getInstance()->nodeReference()->getGeometryAdapter();

			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());

			glPushMatrix();

			glMultMatrixf(&mat[0][0]);

			for (int i = 0; i < geo->getPolygonCount(); i++) {
				glColor3ub(color.r(), color.g(), color.b());
				color.increment();

				glBegin(GL_POLYGON);
				for (int j = 0; j < geo->getPolygonDegree(i); j++) {
					glVertex3f(
						geo->getVertex(geo->getPolygonVertexIndex(i, j)).x,
						geo->getVertex(geo->getPolygonVertexIndex(i, j)).y,
						geo->getVertex(geo->getPolygonVertexIndex(i, j)).z);
				}
				glEnd();
			}
		}
	}

	glDisable(GL_DEPTH_TEST);
}

glm::ivec4 DfxQViewportRenderGL_2_1::colorAtPosition(int x, int y, int height) {
	GLubyte pixel[4];
	glReadPixels(x, height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	return glm::ivec4(pixel[0], pixel[1], pixel[2], pixel[3]);
}

std::vector<DfxColorField> DfxQViewportRenderGL_2_1::colorAtPosition(int x, int y, int w, int h, int height) {
	
	std::vector<DfxColorField> output;
	std::unordered_map<unsigned int, DfxColorField> map;
	int arrSize = 4 * w * h;
	GLubyte *pixel = new GLubyte[4 * w * h];
	glReadPixels(x, height - y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	for (int i = 0; i < arrSize / 4; i++) {
		int curr = 4 * i;
		DfxColorField cf(pixel[curr], pixel[curr + 1], pixel[curr + 2]);
		std::pair<unsigned int, DfxColorField> p(cf.value(), cf);
		map.insert(p);
	}

	for (auto cf : map) output.push_back(cf.second);

	delete[] pixel;

	return output;
}

float DfxQViewportRenderGL_2_1::depthAtPosition(int x, int y, int height) {
	GLfloat dz = 0.f;
	glReadPixels(x, height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);
	return dz;
}

void DfxQViewportRenderGL_2_1::resize(int width, int height) {
	initializeOpenGLFunctions();

	glClearColor(getBackgroundColor().x,
				 getBackgroundColor().y,
				 getBackgroundColor().z,
				 1.0f);
}

//void DfxQViewportRenderGL_2_1::renderWidgets() {
//
//}

glm::mat4 DfxQViewportRenderGL_2_1::getProjectionMatrix() {
	return projMat;
}

glm::mat4 DfxQViewportRenderGL_2_1::getViewMatrix() {
	return viewMat;
}

void DfxQViewportRenderGL_2_1::setProjectionMatrix(glm::mat4 pmat) {
	projMat = pmat;
}

void DfxQViewportRenderGL_2_1::setViewMatrix(glm::mat4 vmat) {
	viewMat = vmat;
}

void DfxQViewportRenderGL_2_1::setWorkGrid(DfxGrid2d grd) {
	wgrid = grd;
}

void DfxQViewportRenderGL_2_1::setBackgroundColor(glm::vec3 bg) {
	backgroundColor = bg;
}

glm::vec3 DfxQViewportRenderGL_2_1::getBackgroundColor() {
	return backgroundColor;
}

void  DfxQViewportRenderGL_2_1::setCameraPosition(glm::vec3 camPos) {
	cameraPosition = camPos;
}

void DfxQViewportRenderGL_2_1::setCameraPath(std::string camPath) {
	cameraPath = camPath;
}

void DfxQViewportRenderGL_2_1::drawWorkGrid() {
	
	float gridLineWidth = 1.6f;
	float subgridGridLineWidth = 1.f;

	float gcolor[] = { .5f, .5f, .5f };
	float subgcolor[] = { .3f, .3f, .3f };

	float sx = wgrid.getOriginX();
	float sy = wgrid.getOriginY();
	float sz = wgrid.getOriginZ();

	float subgridSpacing = wgrid.getSpacing() / (float)wgrid.getSubgrid();

	for (int i = 0; i < wgrid.getWidth(); i++) {

		float dispx = sx + (i * wgrid.getSpacing());
		float dispz = sz + ((wgrid.getHeight() - 1) * wgrid.getSpacing());

		if (i == (wgrid.getWidth() * 0.5))
			glColor3f(0.f, 0.f, 1.f);
		else
			glColor3f(gcolor[0], gcolor[1], gcolor[2]);

		glLineWidth(gridLineWidth);
		glBegin(GL_LINES);
		glVertex3f(dispx, sy, sz);
		glVertex3f(dispx, sy, dispz);
		glEnd();

		// Draw Subgrid
		if (i < (wgrid.getWidth() - 1)) {			
			for (int j = 1; j < wgrid.getSubgrid(); j++) {
				float dispx_sub = dispx + (j * subgridSpacing);
				glLineWidth(subgridGridLineWidth);
				glColor3f(subgcolor[0], subgcolor[1], subgcolor[2]);
				glBegin(GL_LINES);
				glVertex3f(dispx_sub, sy, sz);
				glVertex3f(dispx_sub, sy, dispz);
				glEnd();
			}
		}
	}

	for (int i = 0; i < wgrid.getHeight(); i++) {

		float dispz = sz + (i * wgrid.getSpacing());
		float dispx = sx + ((wgrid.getHeight() - 1) * wgrid.getSpacing());

		if (i == (wgrid.getHeight() * 0.5))
			glColor3f(1.f, 0.f, 0.f);
		else
			glColor3f(gcolor[0], gcolor[1], gcolor[2]);

		glLineWidth(gridLineWidth);
		glBegin(GL_LINES);
		glVertex3f(sx, sy, dispz);
		glVertex3f(dispx, sy, dispz);
		glEnd();

		// Draw Subgrid
		if (i < (wgrid.getHeight() - 1)) {
			for (int j = 1; j < wgrid.getSubgrid(); j++) {
				float dispz_sub = dispz + (j * subgridSpacing);
				glLineWidth(subgridGridLineWidth);
				glColor3f(subgcolor[0], subgcolor[1], subgcolor[2]);
				glBegin(GL_LINES);
				glVertex3f(sx, sy, dispz_sub);
				glVertex3f(dispx, sy, dispz_sub);
				glEnd();
			}
		}
	}
	
}

void DfxQViewportRenderGL_2_1::renderGeoEdges(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color) {
	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();
	glColor3ub(color.x, color.y, color.z);
	glBegin(GL_LINES);
	for (int i = 0; i < geo->getEdgeCount(); i++) {

		auto pindices = geo->getEdgeVerticeIndices(i);

		glVertex3f(
			geo->getVertex(pindices.first).x,
			geo->getVertex(pindices.first).y,
			geo->getVertex(pindices.first).z);

		glVertex3f(
			geo->getVertex(pindices.second).x,
			geo->getVertex(pindices.second).y,
			geo->getVertex(pindices.second).z);

	}
	glEnd();
}

void DfxQViewportRenderGL_2_1::renderGeoPolys(deepsg::SgNode *node, bool face_normal, glm::ivec3 color, bool ignoreSelection) {

	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

	if (geo != nullptr) {
		DfxObjectHandler handler;
		DfxObjectCache *cache;

		if (geo->type() == DfxGeoAdapterType::DYNA_MESH) {
			auto dynaMesh = (DfxDynaMesh*)((deepsg::SgGeoNode*)node)->getDynaMesh();

			if (dynaMesh != nullptr) {
				cache = dynaMesh->cache();
			}
		}
		else {

			if (handler.fetch(node->referencePath())) {
				cache = handler.objectCache();
			}
		}

		if (cache != nullptr) {
			cache->drawMesh(_shadingMode);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void DfxQViewportRenderGL_2_1::renderGeoPolys(DfxFluentSimplexObject *flObj) {

	auto cache = flObj->cache();

	if (cache != nullptr) {
		cache->drawMesh(_shadingMode);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DfxQViewportRenderGL_2_1::renderInstancedGeoPolys(deepsg::SgNode *node, bool face_normal, glm::ivec3 color , bool ignoreSelection,
	std::vector<glm::mat4> xforms ) {

	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

	if (geo != nullptr) {
		DfxObjectHandler handler;
		DfxObjectCache *cache;

		if (geo->type() == DfxGeoAdapterType::DYNA_MESH) {
			auto dynaMesh = (DfxDynaMesh*)((deepsg::SgGeoNode*)node)->getDynaMesh();

			if (dynaMesh != nullptr) {
				cache = dynaMesh->cache();
			}
		}
		else {

			if (handler.fetch(node->referencePath())) {
				cache = handler.objectCache();
			}
		}

		if (cache != nullptr) {
			cache->drawMesh(_shadingMode, 1.0f, xforms, true);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}


void DfxQViewportRenderGL_2_1::renderGeoPolys(deepsg::SgNode *node, deepsg::SgStateInfo *info, bool face_normal, glm::ivec3 color) {
	renderGeoPolys(node, face_normal, color, false);
}

void DfxQViewportRenderGL_2_1::renderGeoPolysWire(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color) {

	DfxObjectHandler handler;

	if (handler.fetch(node->referencePath())) {
		auto cache = handler.fetchObjectCache();

		if (cache != nullptr) {
			glColor3ub(100, 100, 100);
			cache->drawMesh(DfxQViewportRenderInterface::ShadeMode::WIREFRAME);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	
}

void DfxQViewportRenderGL_2_1::renderGeoPolysWire2(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color, float lineWidth) {
	DfxObjectHandler handler;

	if (handler.fetch(node->referencePath())) {
		auto cache = handler.fetchObjectCache();

		if (cache != nullptr) {
			glColor3ub(color.x, color.y, color.z);
			cache->drawMesh(DfxQViewportRenderInterface::ShadeMode::WIREFRAME, lineWidth);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

}

void DfxQViewportRenderGL_2_1::drawSelectionMarquee() {
	
	if (selectionMarquee->isActive()) {

		int w = selectionMarquee->canvasWidth();
		int h = selectionMarquee->canvasHeight();
		int isx = selectionMarquee->startX();
		int isy = selectionMarquee->startY();
		int iex = selectionMarquee->endX();
		int iey = selectionMarquee->endY();
		float sx = (float)selectionMarquee->startX();
		float sy = (float)selectionMarquee->startY();
		float ex = (float)selectionMarquee->endX();
		float ey = (float)selectionMarquee->endY();
		glm::ivec4 fc = selectionMarquee->fillColor();
		glm::ivec4 sc = selectionMarquee->strokeColor();
		GLfloat dz = 0.f;
		glm::mat4 orthoMat(1);

		GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;

		float viewPhi = -37.75;
		float viewTheta = -27;
		float viewLamda = 0.f;
		float viewDepth = -8.0;
		float viewPanX = 0.0f;
		float viewPanY = 0.0f;

		auto perspectiveMat = glm::perspective(45.0f, (float)w / h, 0.05f, 500.0f);

		viewMat  = glm::translate(glm::mat4(1.0f), glm::vec3(viewPanX, -viewPanY, viewDepth));
		viewMat *= glm::rotate(glm::radians(-viewTheta), glm::vec3(1.0f, 0.0f, 0.0f));
		viewMat *= glm::rotate(glm::radians(viewPhi), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMat *= glm::rotate(glm::radians(viewLamda), glm::vec3(0.0f, 0.0f, 1.0f));

		if (w <= h) {
			orthoMat = glm::ortho(-1.f, 1.f, -1.f / aspectRatio, 1.00f / aspectRatio, -100.f, 100.f);
		}
		else {
			orthoMat = glm::ortho(-1.f * aspectRatio, 1.00f * aspectRatio, -1.f, 1.f, -100.1f, 100.f);
		}

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(&orthoMat[0][0]);	

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(&viewMat[0][0]);

		glReadPixels(isx, h - isy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);
		auto a = glm::unProject(glm::vec3(isx, h - isy, dz), viewMat, orthoMat, glm::ivec4(0, 0, w, h));

		glReadPixels(iex, h - isy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);
		auto b = glm::unProject(glm::vec3(iex, h - isy, dz), viewMat, orthoMat, glm::ivec4(0, 0, w, h));

		glReadPixels(iex, h - iey, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);
		auto c = glm::unProject(glm::vec3(iex, h - iey, dz), viewMat, orthoMat, glm::ivec4(0, 0, w, h));

		glReadPixels(isx, h - iey, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dz);
		auto d = glm::unProject(glm::vec3(isx, h - iey, dz), viewMat, orthoMat, glm::ivec4(0, 0, w, h));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(fc.x, fc.y, fc.z, fc.w);
		glBegin(GL_QUADS);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(d.x, d.y, d.z);
		glEnd();

		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x00FF);
		glColor4ub(sc.x, sc.y, sc.z, sc.w);
		glLineWidth(0.5);
		glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(b.x, b.y, b.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(c.x, c.y, c.z);
		glVertex3f(d.x, d.y, d.z);
		glVertex3f(d.x, d.y, d.z);
		glVertex3f(a.x, a.y, a.z);
		glEnd();
		glLineWidth(1.f);
		glDisable(GL_LINE_STIPPLE);

		glDisable(GL_BLEND);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

void DfxQViewportRenderGL_2_1::renderGeoVerts(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 selectedColor, glm::ivec3 color) {
	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();
	
	//GLfloat *point = new GLfloat[geo->getVertexCount() * 3];

	//for (int i = 0; i < geo->getVertexCount(); i++) {
	//	int curr = 3 * i;
	//	point[curr] = geo->getVertex(i).x;
	//	point[curr+1] = geo->getVertex(i).y;
	//	point[curr+2] = geo->getVertex(i).z;
	//}

	//
	//// activate and specify pointer to vertex array
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, point);

	//// draw a cube
	//glColor3ub(color.x, color.y, color.z);
	//glDrawArrays(GL_POINTS, 0, geo->getVertexCount());

	//// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);

	//delete[] point;
	for (int i = 0; i < geo->getVertexCount(); i++) {
		glPointSize(4.f);
		if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
			if (DfxMeshSelectionModel::getInstance() != nullptr) {
				if (DfxMeshSelectionModel::getInstance()->hasVertexIndex(i)) {
					glPointSize(5.f);
					glColor3ub(selectedColor.x, selectedColor.y, selectedColor.z);
				}
				else
				{
					glColor3ub(color.x, color.y, color.z);
				}
			}
		}
		else {
			glColor3ub(color.x, color.y, color.z);
		}
		
		glBegin(GL_POINTS);
		glVertex3f(geo->getVertex(i).x,
				   geo->getVertex(i).y,
				   geo->getVertex(i).z);
		
		glEnd();
	}
}

void DfxQViewportRenderGL_2_1::renderGeoEdges(deepsg::SgNode *node, deepsg::SgStateInfo *info,  bool only_selected, glm::ivec3 selectedColor, glm::ivec3 color) {
	
	bool render;
	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();
	
	glBegin(GL_LINES);
	
	for (int i = 0; i < geo->getEdgeCount(); i++) {
		if (only_selected) {
			render = false;
			if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
				if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr &&
					!DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

					if (DfxMeshSelectionModel::getInstance()->hasEdgeIndex(i)) render = true;
				}
				
			}
		}
		else {
			render = true;
		}

		if (render) {
			glColor3ub(selectedColor.x, selectedColor.y, selectedColor.z);
		}
		else {
			glColor3ub(color.x, color.y, color.z);
		}

		auto pindices = geo->getEdgeVerticeIndices(i);

		glVertex3f(
			geo->getVertex(pindices.first).x,
			geo->getVertex(pindices.first).y,
			geo->getVertex(pindices.first).z);

		glVertex3f(
			geo->getVertex(pindices.second).x,
			geo->getVertex(pindices.second).y,
			geo->getVertex(pindices.second).z);
	}

	glEnd();
}

// static int _check = 0;
void DfxQViewportRenderGL_2_1::renderGeoFaces(deepsg::SgNode *node, deepsg::SgStateInfo *info) {
	auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

	bool render = false;
	
	if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr &&
			!DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {
				
			DfxObjectHandler handler;

			if (handler.fetch(node->referencePath())) {
				auto cache = handler.fetchObjectCache();

				if (cache != nullptr) {
					glColor4ub(255, 153, 85, 127);
					cache->drawMeshElements(DfxMeshSelectionModel::getInstance()->selectedFaceTriangleIndices());

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}
			}
				
		}
	}
	
}

//void DfxQViewportRenderGL_2_1::renderGeoPolys(deepsg::SgNode *node, bool face_normal, glm::ivec3 color, bool ignoreSelection) {
//	
//}

void DfxQViewportRenderGL_2_1::renderGeo(deepsg::SgNode *node, deepsg::SgStateInfo *info) {

	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

		auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

		if (geo != nullptr) {
			glm::mat4 finalTransform(1.f);

			if (!node->transformOverrides().empty()) {

				deepsg::SgTransformOverrideInfo ov_info;

				auto fps = DfxTimelineManager::getInstance()->fps();
				auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
				double time = (double)current_frame / (double)fps;

				ov_info.fps = fps;
				ov_info.frame = current_frame;
				ov_info.time = time;
				ov_info.globalTransform = info->transformMatrix();

				for (auto xoverride : node->transformOverrides()) {
					finalTransform *= xoverride->evaluate(ov_info);
				}
			}
			else {
				finalTransform = info->transformMatrix();
			}

			applyShadedSolidState();
			glPushMatrix();

			glMultMatrixf(&finalTransform[0][0]);

			if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {

				if (DfxSceneSelectionModel::getInstance()->isSelected(node)) {

					glEnable(GL_STENCIL_TEST);
					glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

					glStencilMask(0xFF);
					glClear(GL_STENCIL_BUFFER_BIT);

					glStencilFunc(GL_ALWAYS, 1, 0xFF);

					
					
					renderGeoPolys(node, info, true, glm::ivec3(142, 142, 142));
					undoShadedSolidState();

					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);

					renderGeoPolysWire2(node, info, glm::ivec3(255, 102, 0), 3.f);

					glDisable(GL_STENCIL_TEST);
				}
				else {
			
					//applyShadedSolidState();	

					renderGeoPolys(node, info, true, glm::ivec3(142, 142, 142));
					undoShadedSolidState();
				}

			}
			else {

				//applyShadedSolidState();

				renderGeoPolys(node, info, true, glm::ivec3(142, 142, 142));
				undoShadedSolidState();

				if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT ||
					DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {

					if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
						if (DfxMeshSelectionModel::getInstance()->nodeReference()->getID() == node->getID()) {
							renderGeoFaces(node, info);
						}
					}

					if (DfxMeshSelectionModel::getInstance()->nodeReference()->getID() == node->getID()) {
						applyWireSolidState();
						renderGeoPolysWire(node, info, glm::ivec3(50, 50, 50));
						undoWireSolidState();
					}
				}

				if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
					if (DfxMeshSelectionModel::getInstance()->nodeReference()->getID() == node->getID()) {
						applyVertexState();
						renderGeoVerts(node, info, glm::ivec3(255, 102, 0), glm::ivec3(100, 100, 100));
						undoVertexState();
					}
				}
				else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
					if (DfxMeshSelectionModel::getInstance()->nodeReference()->getID() == node->getID()) {
						applySelectedEdgeState();
						renderGeoEdges(node, info, true, glm::ivec3(255, 102, 0), glm::ivec3(50, 50, 50));
						undoSelectedEdgeState();
					}
				}

			}

			glPopMatrix();
		}
	}
}

void DfxQViewportRenderGL_2_1::renderGeoColorOnly(deepsg::SgNode *node, deepsg::SgStateInfo *info) {
	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

		DfxObjectHandler handler;
		auto geoNode = (deepsg::SgGeoNode*)node;
		auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

		if (geo != nullptr) {
			if (handler.fetch(node->referencePath())) {
				auto cache = handler.fetchObjectCache();

				glPushMatrix();

				glMultMatrixf(&info->transformMatrix()[0][0]);

				if (cache != nullptr) {
					auto color = geoNode->getColor();
					cache->drawDiffuse(color.x, color.y, color.z);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				glPopMatrix();
			}
		}
	}
}

void DfxQViewportRenderGL_2_1::renderGeoColorOnly(deepsg::SgNode *node, glm::ivec3 color, deepsg::SgStateInfo *info) {

	if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

		DfxObjectHandler handler;
		auto geoNode = (deepsg::SgGeoNode*)node;
		auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

		if (geo != nullptr) {
			if (handler.fetch(node->referencePath())) {
				auto cache = handler.fetchObjectCache();

				glPushMatrix();

				glMultMatrixf(&info->transformMatrix()[0][0]);

				if (cache != nullptr) {
					cache->drawDiffuse(color.x, color.y, color.z);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				}

				glPopMatrix();
			}
		}
	}

}

void DfxQViewportRenderGL_2_1::renderWidget(std::vector<DfxColorID> colors) {

	if (DfxWidgetManager::getInstance()->isWidgetActive() && 
	    !DfxWidgetManager::getInstance()->isNull()) {

		bool doRenderWidget = true;

		auto widgetNode = DfxWidgetManager::getInstance()->getWidget();

		deepsg::SgXForm parentTrans = DfxWidgetManager::getInstance()->getTransform();
		
		DfxLxCameraHandler handler;
		auto rc = handler.fetch(cameraPath);

		if (rc) {
			auto snode = handler.shapeNode();
			glm::vec3 sh_pos = snode->position();
			glm::vec3 sg_pos = parentTrans.position();

			float dist = glm::distance(sh_pos, sg_pos);

			if (dist < 0.0005) doRenderWidget = false;
		}

		if (doRenderWidget) {
			int counter = 0;
			for (auto buffer : widgetNode->widget()->handles()) {
				glPushMatrix();

				auto transMat = widgetNode->widget()->handleTransform(counter, parentTrans);
				glMultMatrixf(&transMat[0][0]);

				if (colors.size() == widgetNode->widget()->handles().size()) {
					renderShapeBuffer(buffer, widgetRatio * (glm::distance(parentTrans.position(), cameraPosition)),
						colors[counter].redi(),
						colors[counter].greeni(),
						colors[counter].bluei(),
						true);
				}
				else {
					renderShapeBuffer(buffer, widgetRatio * (glm::distance(parentTrans.position(), cameraPosition)));
				}

				counter++;
				glPopMatrix();
			}
		}
	
	}
}

void DfxQViewportRenderGL_2_1::renderShape(deepsg::SgNode *node, deepsg::SgStateInfo *info) {
	glEnable(GL_DEPTH_TEST);
	if (node->getType() == deepsg::SgNodeType::SHAPE) {

		bool doRenderWidget = true;
		auto shapeNode = (deepsg::SgShapeNode*)node;

		DfxLxCameraHandler handler;
		auto rc = handler.fetch(cameraPath);

		if (rc) {
			auto snode = handler.shapeNode();
			if (snode->getID() == shapeNode->getID()) doRenderWidget = false;
		}

		if (doRenderWidget) {
			glPushMatrix();

			glMultMatrixf(&info->transformMatrix()[0][0]);

			for (auto sh : shapeNode->shape()->shapes()) {

				
				if (sh.isWireframe()) {
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);
					glEnable(GL_LINE_SMOOTH);
					glEnable(GL_CULL_FACE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}

				renderShapeBuffer(sh, shapeNode->shape()->getScale());

				if (sh.isWireframe()) {
					undoWireSolidState();
				}
				
			}

			glPopMatrix();
		}
	}
	else if (node->getType() == deepsg::SgNodeType::NILL) {

		auto nullNode = (DfxNullNode*)node;

		DfxNullHandler handler;

		deepsg::SgShape *shape = nullptr;

		if (handler.fetch(nullNode->referencePath())) {
			auto current_shape = handler.currentShapeIndex();
			shape = nullNode->shape(current_shape);
		}

		if (shape != nullptr) {
			glPushMatrix();

			glMultMatrixf(&info->transformMatrix()[0][0]);

			for (auto sh : shape->shapes()) {

				if (sh.isWireframe()) {
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);
					glEnable(GL_LINE_SMOOTH);
					glEnable(GL_CULL_FACE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}

				renderShapeBuffer(sh, 1.f);

				if (sh.isWireframe()) {
					undoWireSolidState();
				}
			}

			glPopMatrix();
		}
	}
	else if (node->getType() == deepsg::SgNodeType::STAGE_PROP) {

		auto propNode = (deepsg::SgStagePropNode*)node;
		auto shape = propNode->shape();

		if (shape != nullptr) {
			glPushMatrix();

			glMultMatrixf(&info->transformMatrix()[0][0]);

			for (auto sh : shape->shapes()) {

				if (sh.isWireframe()) {
					glDisable(GL_LIGHTING);
					glEnable(GL_BLEND);
					glEnable(GL_LINE_SMOOTH);
					glEnable(GL_CULL_FACE);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}

				renderShapeBuffer(sh, 1.f);

				if (sh.isWireframe()) {
					undoWireSolidState();
				}
			}

			glPopMatrix();
		}
	}

	glDisable(GL_DEPTH_TEST);
}

void DfxQViewportRenderGL_2_1::renderShapeColorOnly(deepsg::SgNode *node, deepsg::SgStateInfo *info) {

	if (node->getType() == deepsg::SgNodeType::SHAPE) {

		auto shapeNode = (deepsg::SgShapeNode*)node;

		glPushMatrix();

		glMultMatrixf(&info->transformMatrix()[0][0]);

		for (auto sh : shapeNode->shape()->shapes()) {
			auto cl = shapeNode->getColor();
			renderShapeBuffer(sh, shapeNode->shape()->getScale(), cl.x, cl.y, cl.z, true);
		}

		glPopMatrix();
	} 
	else if (node->getType() == deepsg::SgNodeType::NILL) {
		auto nullNode = (DfxNullNode*)node;

		DfxNullHandler handler;

		deepsg::SgShape *shape = nullptr;

		if (handler.fetch(nullNode->referencePath())) {
			auto current_shape = handler.currentShapeIndex();
			shape = nullNode->shape(current_shape);
		}

		if (shape != nullptr) {
			glPushMatrix();

			glMultMatrixf(&info->transformMatrix()[0][0]);

			for (auto sh : shape->shapes()) {
				auto cl = nullNode->getColor();
				renderShapeBuffer(sh, 1.f, cl.x, cl.y, cl.z, true);
			}

			glPopMatrix();
		}
	}
	else if (node->getType() == deepsg::SgNodeType::STAGE_PROP) {

		auto propNode = (deepsg::SgStagePropNode*)node;

		glPushMatrix();

		glMultMatrixf(&info->transformMatrix()[0][0]);

		auto shape  =  propNode->shape();

		if (shape != nullptr) {
			for (auto sh : shape->shapes()) {
				auto cl = propNode->getColor();
				renderShapeBuffer(sh, propNode->shape()->getScale(), cl.x, cl.y, cl.z, true);
			}
		}

		glPopMatrix();
	}
}

void DfxQViewportRenderGL_2_1::renderShapeBuffer(deepsg::SgShapeBuffer buffer, float scale, int r, int g, int b, bool overrideColor) {

	for (auto cmd : buffer.buffer()) {

		switch (cmd.type) {
		case deepsg::SgCommandType::SET_COLOR:
		{
			if(overrideColor)
				glColor3ub(r, g, b);
			else
				glColor3ub(cmd.r, cmd.g, cmd.b);
		}
			break;
		case deepsg::SgCommandType::SET_LINE_WIDTH:
		{
			glLineWidth(cmd.size);
		}
			break;
		case deepsg::SgCommandType::SET_POINT_SIZE:
		{
			glPointSize(cmd.size);
		}
			break;
		case deepsg::SgCommandType::DRAW_LINE:
		{
			glBegin(GL_LINES);
			for (auto p : cmd.points) glVertex3f( p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
			break;
		case deepsg::SgCommandType::DRAW_STIPPLED_LINE:
		{
			glLineStipple(1, 0xAAAA);
			glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}
			break;
		case deepsg::SgCommandType::DRAW_POINT:
		{
			glBegin(GL_POINT);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
			break;
		case deepsg::SgCommandType::DRAW_TRIANGLE:
		{
			glBegin(GL_TRIANGLES);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
			break;
		case deepsg::SgCommandType::DRAW_QUAD:
		{
			glBegin(GL_QUADS);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
			break;
		case deepsg::SgCommandType::DRAW_POLYGON:
		{
			glBegin(GL_POLYGON);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
		case deepsg::SgCommandType::DRAW_QUAD_STRIP:
		{
			glBegin(GL_QUAD_STRIP);
			for (auto p : cmd.points) glVertex3f(p.x * scale, p.y * scale, p.z * scale);
			glEnd();
		}
			break;
		default:
			break;
		};
	}
}

void DfxQViewportRenderGL_2_1::drawCameraRenderRegion(int width, int height) {

	auto any = DfxSpace::getInstance()->findAbsolute(cameraPath);

	if (any != nullptr) {
		fl::DataObject *dao = nullptr;

		if (any->type() == fl::type::DATA_OBJECT) {
			dao = any->rawData<fl::DataObject>();
		}
		else {
			return;
		}

		if (!dao->hasProperty("width")) return;
		if (!dao->hasProperty("height")) return;
		if (!dao->hasProperty("field_of_view")) return;
		if (!dao->hasProperty("render_target")) return;
		if (!dao->hasProperty("xform")) return;

		auto fov = dao->property("field_of_view")->data<float>();
		auto render_target = dao->property("render_target")->data<unsigned int>();

		if (render_target == DfxRenderTarget::OSPRAY) fov += 15.f;

		float cam_w = dao->property("width")->data<int>();
		float cam_h = dao->property("height")->data<int>();
		
		int w = width;
		int h = height;

		static float proj[16];

		glMatrixMode(GL_PROJECTION);
		auto _proj = glm::perspective(glm::radians(fov), cam_w / cam_h, 0.05f, 500.0f);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(&_proj[0][0]);
		glGetFloatv(GL_PROJECTION_MATRIX, proj);
		glPopMatrix();	

		double _nearScale = 1.f;
		if (cam_w > cam_h) _nearScale = cam_h / cam_w;

		const double _near = proj[11] / (proj[10] - 1.0);

		const double nLeft  = _near * _nearScale *(proj[2] - 1.0) / proj[0];
		const double nRight = _near * _nearScale *(1.0 + proj[2]) / proj[0];
		const double nTop   = _near * _nearScale *(1.0 + proj[6]) / proj[5];
		const double nBottom = _near * _nearScale *(proj[6] - 1.0) / proj[5];

		glMatrixMode(GL_MODELVIEW);
		glLineStipple(1, 0x00FF);
		glEnable(GL_LINE_STIPPLE);
		glLineWidth(2.0);
		glPushMatrix();
		glMultMatrixf(&glm::inverse(viewMat)[0][0]);
		glBegin(GL_LINES);

		glColor3f(1.f, 1.f, 1.f);

		glVertex3f(nLeft, nBottom, -_near);
		glVertex3f(nRight, nBottom, -_near);

		glVertex3f(nRight, nTop, -_near);
		glVertex3f(nLeft, nTop, -_near);

		glVertex3f(nLeft, nTop, -_near);
		glVertex3f(nLeft, nBottom, -_near);

		glVertex3f(nRight, nTop, -_near);
		glVertex3f(nRight, nBottom, -_near);

		glEnd();
		glPopMatrix();

		glDisable(GL_LINE_STIPPLE);
	}
}

void DfxQViewportRenderGL_2_1::renderInstances() {

	for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

		instancer->begin();

		if (instancer->isEnabled()) {
			
			auto limit = std::min((unsigned int)50000, instancer->limit());

			unsigned int instanceCount = instancer->instanceCount();
			if (limit > 0) instanceCount = std::min(instanceCount, limit);

			std::vector<unsigned int> oneMillionShuffle;

			oneMillionShuffle.resize(instanceCount);
			for (unsigned int inx = 0; inx < instanceCount; inx++) {
				oneMillionShuffle[inx] = inx;
			}

			std::shuffle(oneMillionShuffle.begin(), oneMillionShuffle.end(), std::mt19937_64(12345));

			if (instancer->instanceProxyType() == DfxInstancerBase::AABB_PROXY) {

				if (instancer->instanceCount() > 0) {
					
					std::unique_ptr<float> pbuffer(new float[instanceCount * 72]);

					#pragma omp parallel default(shared)
					{
						#pragma omp for schedule(static)
						for (long long int counter = 0; counter < instanceCount; counter++) {
							unsigned int inx = 0;

							if (limit > 0) inx = oneMillionShuffle[counter];
							else inx = counter;

							auto node = instancer->instanceShape(inx);

							size_t ot = inx * 72;

							glm::mat4 xform(1.f);

							if (instancer->useTransformMatrix(inx)) xform = instancer->instanceTransform(inx);
							else xform = instancer->finalTransform(inx);

							auto aabb = instancer->instanceShapeAABB(inx);

							auto min = glm::vec3(xform * glm::vec4(aabb.xmin, aabb.ymin, aabb.zmin, 1.f));
							auto max = glm::vec3(xform * glm::vec4(aabb.xmax, aabb.ymax, aabb.zmax, 1.f));

							// Bottom
							pbuffer.get()[ot] = min.x; pbuffer.get()[ot + 1] = min.y; pbuffer.get()[ot + 2] = min.z;
							pbuffer.get()[ot + 3] = max.x; pbuffer.get()[ot + 4] = min.y; pbuffer.get()[ot + 5] = min.z;

							pbuffer.get()[ot + 6] = min.x; pbuffer.get()[ot + 7] = min.y; pbuffer.get()[ot + 8] = min.z;
							pbuffer.get()[ot + 9] = min.x; pbuffer.get()[ot + 10] = min.y; pbuffer.get()[ot + 11] = max.z;

							pbuffer.get()[ot + 12] = max.x; pbuffer.get()[ot + 13] = min.y; pbuffer.get()[ot + 14] = max.z;
							pbuffer.get()[ot + 15] = min.x; pbuffer.get()[ot + 16] = min.y; pbuffer.get()[ot + 17] = max.z;

							pbuffer.get()[ot + 18] = max.x; pbuffer.get()[ot + 19] = min.y; pbuffer.get()[ot + 20] = max.z;
							pbuffer.get()[ot + 21] = max.x; pbuffer.get()[ot + 22] = min.y; pbuffer.get()[ot + 23] = min.z;

							// Top
							pbuffer.get()[ot + 24] = min.x; pbuffer.get()[ot + 25] = max.y; pbuffer.get()[ot + 26] = min.z;
							pbuffer.get()[ot + 27] = max.x; pbuffer.get()[ot + 28] = max.y; pbuffer.get()[ot + 29] = min.z;

							pbuffer.get()[ot + 30] = min.x; pbuffer.get()[ot + 31] = max.y; pbuffer.get()[ot + 32] = min.z;
							pbuffer.get()[ot + 33] = min.x; pbuffer.get()[ot + 34] = max.y; pbuffer.get()[ot + 35] = max.z;

							pbuffer.get()[ot + 36] = max.x; pbuffer.get()[ot + 37] = max.y; pbuffer.get()[ot + 38] = max.z;
							pbuffer.get()[ot + 39] = min.x; pbuffer.get()[ot + 40] = max.y; pbuffer.get()[ot + 41] = max.z;

							pbuffer.get()[ot + 42] = max.x; pbuffer.get()[ot + 43] = max.y; pbuffer.get()[ot + 44] = max.z;
							pbuffer.get()[ot + 45] = max.x; pbuffer.get()[ot + 46] = max.y; pbuffer.get()[ot + 47] = min.z;

							// Sides
							pbuffer.get()[ot + 48] = min.x; pbuffer.get()[ot + 49] = min.y; pbuffer.get()[ot + 50] = min.z;
							pbuffer.get()[ot + 51] = min.x; pbuffer.get()[ot + 52] = max.y; pbuffer.get()[ot + 53] = min.z;

							pbuffer.get()[ot + 54] = max.x; pbuffer.get()[ot + 55] = min.y; pbuffer.get()[ot + 56] = min.z;
							pbuffer.get()[ot + 57] = max.x; pbuffer.get()[ot + 58] = max.y; pbuffer.get()[ot + 59] = min.z;

							pbuffer.get()[ot + 60] = max.x; pbuffer.get()[ot + 61] = min.y; pbuffer.get()[ot + 62] = max.z;
							pbuffer.get()[ot + 63] = max.x; pbuffer.get()[ot + 64] = max.y; pbuffer.get()[ot + 65] = max.z;

							pbuffer.get()[ot + 66] = min.x; pbuffer.get()[ot + 67] = min.y; pbuffer.get()[ot + 68] = max.z;
							pbuffer.get()[ot + 69] = min.x; pbuffer.get()[ot + 70] = max.y; pbuffer.get()[ot + 71] = max.z;

						}
					}

					GLuint pointsVBO;
					glGenBuffers(1, &pointsVBO);

					// Make the new VBO active
					glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float) * instanceCount * 72, pbuffer.get(), GL_STATIC_DRAW);

					glVertexPointer(3, GL_FLOAT, 0, NULL);

					glColor3f(1.f, 0.f, 0.f);

					// Establish array contains vertices 
					glEnableClientState(GL_VERTEX_ARRAY);

					glDrawArrays(GL_LINES, 0, instanceCount * 24);

					glDisableClientState(GL_VERTEX_ARRAY);

					glBindBuffer(GL_ARRAY_BUFFER, 0);
				}
			}
			else if (instancer->instanceProxyType() == DfxInstancerBase::SHAPE_PROXY) {

				std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

				for (size_t inx = 0; inx < instancer->instanceCount(); inx++) {
					glm::mat4 xform(1.f);

					if (instancer->useTransformMatrix(inx)) xform = instancer->instanceTransform(inx);
					else xform = instancer->finalTransform(inx);

					auto node = instancer->instanceShape(inx);

					if (node != nullptr) {

						if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

							auto itr = instanceData.find(node->getID());

							if (itr == instanceData.end()) {
								instanceData.insert(std::pair<size_t, DfxQRenderInstanceInfo>(node->getID(), DfxQRenderInstanceInfo()));
								itr = instanceData.find(node->getID());
								itr->second.node = node;
							}

							if (itr != instanceData.end()) {
								if (!instancer->hidden(inx)) {
									itr->second.xforms.push_back(xform);
								}
							}

						}
					}
				}

				for (auto &info : instanceData) {
					renderInstancedGeoPolys(info.second.node, false, glm::ivec3(142, 142, 142), false, info.second.xforms);
				}
			}
		}

		instancer->end();

	}
	
}

void DfxQViewportRenderGL_2_1::renderDelegates(deepsg::SgNode *node, deepsg::SgStateInfo *info) {

	if (node->getType() == deepsg::SgNodeType::RENDER_DELEGATE) {
		auto rdelegate = (deepsg::SgRenderDelegateNode*)node;

		if (!rdelegate->isInitialized()) rdelegate->initialize();

		rdelegate->render(node, info);
	}

}

void DfxQViewportRenderGL_2_1::renderPointCloud(deepsg::SgNode *node, deepsg::SgStateInfo *info) {

	if (node->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		DfxPointCloudHandler handler;

		if (handler.fetch(node->referencePath())) {

			glm::mat4 finalTransform(1.f);

			auto dao = (fl::DataObject*)handler.dataObject()->rawData<fl::DataObject>();
			auto pcloud = handler.dsPointCloudData();

			auto useTransform = dao->property("parenting")->data<bool>();

			if (useTransform) {
				if (!node->transformOverrides().empty()) {

					deepsg::SgTransformOverrideInfo ov_info;

					auto fps = DfxTimelineManager::getInstance()->fps();
					auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
					double time = (double)current_frame / (double)fps;

					ov_info.fps = fps;
					ov_info.frame = current_frame;
					ov_info.time = time;
					ov_info.globalTransform = info->transformMatrix();

					for (auto xoverride : node->transformOverrides()) {
						finalTransform *= xoverride->evaluate(ov_info);
					}
				}
				else {
					finalTransform = info->transformMatrix();
				}
			}
			
			if (pcloud->size() > 0) {

				GLuint points, colors;
				glGenBuffers(1, &points);
				glGenBuffers(1, &colors);
			
				std::unique_ptr<GLfloat[]> pts_arr;
				std::unique_ptr<GLfloat[]> cls_arr;

				pts_arr.reset(new float[pcloud->size() * 3]);
				cls_arr.reset(new float[pcloud->size() * 4]);

				auto posChannel = pcloud->channel("@position");
				auto colChannel = pcloud->channel("@color");
				auto hiddenChannel = pcloud->channel("@hidden");

				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (long long int i = 0; i < pcloud->size(); i++) {
						int current = i * 3;
						int current_color = i * 4;

						pts_arr.get()[current] = posChannel->at<glm::vec3>()[i].x;
						pts_arr.get()[current + 1] = posChannel->at<glm::vec3>()[i].y;
						pts_arr.get()[current + 2] = posChannel->at<glm::vec3>()[i].z;

						if (colChannel != nullptr) {
							cls_arr.get()[current_color] = colChannel->at<glm::vec3>()[i].x;
							cls_arr.get()[current_color + 1] = colChannel->at<glm::vec3>()[i].y;
							cls_arr.get()[current_color + 2] = colChannel->at<glm::vec3>()[i].z;

							if (hiddenChannel != nullptr) {
								if(hiddenChannel->at<int>()[i] == 1) cls_arr.get()[current_color + 3] = 0.f;
								else cls_arr.get()[current_color + 3] = 1.f;
							}
							else {
								cls_arr.get()[current_color + 3] = 1.f;
							}
						}
						else {
							cls_arr.get()[current_color] = 1.f;
							cls_arr.get()[current_color + 1] = 1.f;
							cls_arr.get()[current_color + 2] = 1.f;

							if (hiddenChannel != nullptr) {
								if (hiddenChannel->at<int>()[i] == 1) 
									cls_arr.get()[current_color + 3] = 0.f;
								else 
									cls_arr.get()[current_color + 3] = 1.f;
							}
							else {
								cls_arr.get()[current_color + 3] = 1.f;
							}
						}
					}
				}

				glPushMatrix();
				glMultMatrixf(&finalTransform[0][0]);

				applyPointCloudPointsState();

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, points);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pcloud->size() * 3, &pts_arr.get()[0], GL_STATIC_DRAW);
				glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

				glBindBuffer(GL_ARRAY_BUFFER, colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * pcloud->size() * 4, &cls_arr.get()[0], GL_STATIC_DRAW);
				glColorPointer(4, GL_FLOAT, 4 * sizeof(float), BUFFER_OFFSET(0));

				/*auto shader = DfxGLSLShaderWrangler::getInstance()->findGLSLProgram("sph_point_sprite_shader");
				if (shader != nullptr) if (shader->ready) {
					shader->program.bind();

					auto pm = shader->program.uniformLocation("projMat");
					auto vm = shader->program.uniformLocation("viewMat");
					auto mm = shader->program.uniformLocation("modMat");

					shader->program.setUniformValue(pm, QMatrix4x4(&glm::transpose(projMat)[0][0]));
					shader->program.setUniformValue(vm, QMatrix4x4(&glm::transpose(viewMat)[0][0]));
					shader->program.setUniformValue(mm, QMatrix4x4(&glm::transpose(finalTransform)[0][0]));
				}*/
				
				glDrawArrays(GL_POINTS, 0, pcloud->size());

				glUseProgram(0);

				glDeleteBuffers(1, &points);
				glDeleteBuffers(1, &colors);

				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);

				undoPointCloudPointsState();

				glPopMatrix();
				glColor3ub(100, 100, 100);
			}
		}
		
	}
}

void DfxQViewportRenderGL_2_1::renderVolume(deepsg::SgNode *node, deepsg::SgStateInfo *info) {
	if (node->getType() == deepsg::SgNodeType::GRID) {

		DfxGridHandler ghandler;

		if (ghandler.fetch(node->referencePath())) {

			auto dao = ghandler.dataObject()->rawData<fl::DataObject>();
			auto cache = ghandler.volumeCache();

			auto voxelPixelSize = dao->property("voxel_pixel_size")->data<float>();
			auto transparency = dao->property("transparency")->data<bool>();

			if (cache != nullptr) {
				glm::mat4 finalTransform(1.f);

				if (cache->isReady()) {
					if (!node->transformOverrides().empty()) {

						deepsg::SgTransformOverrideInfo ov_info;

						auto fps = DfxTimelineManager::getInstance()->fps();
						auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
						double time = (double)current_frame / (double)fps;

						ov_info.fps = fps;
						ov_info.frame = current_frame;
						ov_info.time = time;
						ov_info.globalTransform = info->transformMatrix();

						for (auto xoverride : node->transformOverrides()) {
							finalTransform *= xoverride->evaluate(ov_info);
						}
					}
					else {
						finalTransform = info->transformMatrix();
					}

					glPushMatrix();
					glMultMatrixf(&finalTransform[0][0]);

					if (cache->isReady()) {
						cache->drawVolume(voxelPixelSize, transparency);
					}

					glPopMatrix();
				}
			}
		}
	}
}

void DfxQViewportRenderGL_2_1::renderScene() {
	if (!DfxGLSLShaderWrangler::getInstance()->isCompiledLinked()) DfxGLSLShaderWrangler::getInstance()->compileLink();

	deepsg::SgConditionalTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {
		if (node->isVisible()) {

			renderGeo(node, info);
			renderVolume(node, info);
			renderInstances();
			renderShape(node, info);
			renderPointCloud(node, info);
			renderDelegates(node, info);

			return deepsg::TraversalFlag::CONTINUE;
		}
		else {

			return deepsg::TraversalFlag::IGNORE_CHILDREN;
		}
	};

	deepsg::SgRootNode::getInstance()->traverseSceneConditionally(renderFunc);

	renderWidget(std::vector<DfxColorID>());
}

void DfxQViewportRenderGL_2_1::renderFluentSimplexScene() {
	
	for (auto &p : DfxFluentSetManager::getInstance()->setMap()) {
		if (p.second->isVisible()) {

			if (p.second->type() == DfxFluentSet::SIMPLEX) {
				for (auto index : p.second->indices()) {
					auto simplexObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[index];
					auto xform = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform")[index];

					glPushMatrix();

					glMultMatrixf(&xform[0][0]);

					auto cl = p.second->color();
					glColor3f(cl.x, cl.y, cl.z);
					renderGeoPolys(simplexObj);
					glColor3ub(100, 100, 100);

					glPopMatrix();
				}
			}
			
		}
	}
}

void DfxQViewportRenderGL_2_1::renderFluentStreamScene() {

	size_t stream_size = 0;
	for (auto &p : DfxFluentSetManager::getInstance()->setMap()) {
		if (p.second->isVisible()) {
			if (p.second->type() == DfxFluentSet::STREAM) {
				stream_size += p.second->indices().size();
			}
		}
	}

	if (stream_size > 0) {
		auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

		GLuint points, colors;
		glGenBuffers(1, &points);
		glGenBuffers(1, &colors);

		std::unique_ptr<GLfloat[]> pts_arr;
		std::unique_ptr<GLfloat[]> cls_arr;

		pts_arr.reset(new float[stream_size * 3]);
		cls_arr.reset(new float[stream_size * 3]);

		size_t current = 0;
		for (auto &p : DfxFluentSetManager::getInstance()->setMap()) {
			if (p.second->isVisible()) {
				if (p.second->type() == DfxFluentSet::STREAM) {

					auto color = p.second->color();
					for (long long int i = 0; i < p.second->indices().size(); i++) {
		
						pts_arr.get()[current] = positionStream[p.second->indices()[i]].x;
						pts_arr.get()[current + 1] = positionStream[p.second->indices()[i]].y;
						pts_arr.get()[current + 2] = positionStream[p.second->indices()[i]].z;

						cls_arr.get()[current] = color.x;
						cls_arr.get()[current + 1] = color.y;
						cls_arr.get()[current + 2] = color.z;
						
						current += 3;
					}
					
				}

			}
		}

		applyPointCloudPointsState();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, points);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stream_size * 3, &pts_arr.get()[0], GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stream_size * 3, &cls_arr.get()[0], GL_STATIC_DRAW);
		glColorPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

		glDrawArrays(GL_POINTS, 0, stream_size);

		glDeleteBuffers(1, &points);
		glDeleteBuffers(1, &colors);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		undoPointCloudPointsState();
		glColor3ub(100, 100, 100);
	}
}

void DfxQViewportRenderGL_2_1::renderFluentSimplexCacheTrackScene() {
	for (auto track : DfxCacheTrackManager::getInstance()->tracks()) {
		auto snapshot = track->snapshot();

		for (auto set : snapshot->sets()) {
			if (set->isVisible()) {

				if (set->type() == DfxFluentSet::SIMPLEX) {
					for (auto index : set->indices()) {

						auto simplexObj = snapshot->simplexObjects()[index];
						auto xform = snapshot->simplexAttributes().at<glm::mat4>("@xform")[index];;

						glPushMatrix();

						glMultMatrixf(&xform[0][0]);
					
						auto cl = set->color();
						glColor3f(cl.x, cl.y, cl.z);
						renderGeoPolys(simplexObj);
						glColor3ub(100, 100, 100);

						glPopMatrix();
					}
				}
				else {

				}

			}
		}
	}
}

void DfxQViewportRenderGL_2_1::renderFluentStreamCacheTrackScene() {
	for (auto track : DfxCacheTrackManager::getInstance()->tracks()) {
		auto snapshot = track->snapshot();
		
		size_t stream_size = 0;
		for (auto set : snapshot->sets()) {
			if (set->isVisible()) {
				if (set->type() == DfxFluentSet::STREAM) {
					stream_size += set->indices().size();
				}
			}
		}

		if (stream_size > 0) {
			auto positionStream = snapshot->streamAttributes().at<glm::vec3>("@position");

			GLuint points, colors;
			glGenBuffers(1, &points);
			glGenBuffers(1, &colors);

			std::unique_ptr<GLfloat[]> pts_arr;
			std::unique_ptr<GLfloat[]> cls_arr;

			pts_arr.reset(new float[stream_size * 3]);
			cls_arr.reset(new float[stream_size * 3]);

			size_t current = 0;
			for (auto set : snapshot->sets()) {
				if (set->isVisible()) {
					if (set->type() == DfxFluentSet::STREAM) {

						auto color = set->color();
						for (long long int i = 0; i < set->indices().size(); i++) {

							pts_arr.get()[current] = positionStream[set->indices()[i]].x;
							pts_arr.get()[current + 1] = positionStream[set->indices()[i]].y;
							pts_arr.get()[current + 2] = positionStream[set->indices()[i]].z;

							cls_arr.get()[current] = color.x;
							cls_arr.get()[current + 1] = color.y;
							cls_arr.get()[current + 2] = color.z;

							current += 3;
						}

					}

				}
			}

			applyPointCloudPointsState();

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glBindBuffer(GL_ARRAY_BUFFER, points);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stream_size * 3, &pts_arr.get()[0], GL_STATIC_DRAW);
			glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

			glBindBuffer(GL_ARRAY_BUFFER, colors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stream_size * 3, &cls_arr.get()[0], GL_STATIC_DRAW);
			glColorPointer(3, GL_FLOAT, 3 * sizeof(float), BUFFER_OFFSET(0));

			glDrawArrays(GL_POINTS, 0, stream_size);

			glDeleteBuffers(1, &points);
			glDeleteBuffers(1, &colors);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);

			undoPointCloudPointsState();
			glColor3ub(100, 100, 100);
		}
	}
}