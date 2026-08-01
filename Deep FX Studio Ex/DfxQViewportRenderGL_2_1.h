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

#include <qopenglfunctions_3_2_compatibility.h>
#include <SgRootNode.h>
#include <SgShapeBuffer.h>
#include <SgShapeNode.h>
#include "DfxGridHandler.h"
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <unordered_map>
#include <SgTransformOverride.h>
#include <SgRenderDelegateNode.h>
#include "DfxCacheTrackManager.h"
#include "DfxMathUtil.h"
#include "DfxObjectLoader.h"
#include "DfxQViewportRenderInterface.h"
#include "DfxSceneSelectionModel.h"
#include "DfxWidgetManager.h"
#include "DfxMode.h"
#include "DfxMeshSelectionModel.h"
#include "DfxColorField.h"
#include "DfxSelectionMarquee.h"
#include "DfxLxCameraHandler.h"
#include "DfxNullHandler.h"
#include "DfxObjectHandler.h"
#include "DfxOpenGLCacheManager.h"
#include "DfxTimelineManager.h"
#include "DfxInstanceManager.h"
#include "DfxQRenderInstanceInfo.h"
#include "DfxFluentSimplexObject.h"
#include "DfxUtil.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentSetManager.h"


class DfxQViewportRenderGL_2_1 : public DfxQViewportRenderInterface, public QOpenGLFunctions_3_2_Compatibility {
private:

	DfxGrid2d wgrid;
	glm::mat4 projMat;
	glm::mat4 viewMat;
	glm::mat4 cameraTransform;
	glm::vec3 backgroundColor;
	glm::vec3 cameraPosition;
	std::string cameraPath;
	float widgetRatio;

	ShadeMode _shadingMode;
	//ds::Mesh *msh;

private:

	void applyShadedSolidState();
	void undoShadedSolidState();

	void applyWireSolidState();
	void undoWireSolidState();

	void applyVertexState();
	void undoVertexState();

	void applySelectedEdgeState();
	void undoSelectedEdgeState();

	void applySelectedObjectSolidState();
	void undoSelectedObjectSolidState();

	void applyGridDrawingState();
	void undoGridDrawingState();

	void applyPointCloudPointsState();
	void undoPointCloudPointsState();

	void applySolidOutlineState();
	void undoSolidOutlineState();

	void renderScene();
	void renderFluentSimplexScene();
	void renderFluentStreamScene();
	void renderFluentSimplexCacheTrackScene();
	void renderFluentStreamCacheTrackScene();
	void renderShapeBuffer(deepsg::SgShapeBuffer buffer, float scale,  int r=0, int g=0, int b=0, bool overrideColor=false);
	void renderGeo(deepsg::SgNode *node, deepsg::SgStateInfo *info);
	void renderGeoColorOnly(deepsg::SgNode *node, deepsg::SgStateInfo *info);
	void renderGeoColorOnly(deepsg::SgNode *node, glm::ivec3 color, deepsg::SgStateInfo *info);

	void renderGeoEdges(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color=glm::ivec3(0));
	void renderGeoPolys(deepsg::SgNode *node, deepsg::SgStateInfo *info, bool face_normal=true, glm::ivec3 color = glm::ivec3(0));
	void renderGeoPolys(deepsg::SgNode *node, bool face_normal = true, glm::ivec3 color = glm::ivec3(0), bool ignoreSelection = true);
	void renderGeoPolys(DfxFluentSimplexObject *flObj);
	void renderInstancedGeoPolys(deepsg::SgNode *node, bool face_normal = true, glm::ivec3 color = glm::ivec3(0), bool ignoreSelection = true, 
		std::vector<glm::mat4> xforms = std::vector<glm::mat4>());
	void renderGeoPolysWire(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color = glm::ivec3(0));
	void renderGeoPolysWire2(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 color = glm::ivec3(0), float lineWidth = 2.f);
	void renderGeoVerts(deepsg::SgNode *node, deepsg::SgStateInfo *info, glm::ivec3 selectedColor = glm::ivec3(0), glm::ivec3 color = glm::ivec3(0));
	void renderGeoEdges(deepsg::SgNode *node, deepsg::SgStateInfo *info, bool only_selected=false, glm::ivec3 selectedColor = glm::ivec3(0), glm::ivec3 color = glm::ivec3(0));
	void renderGeoFaces(deepsg::SgNode *node, deepsg::SgStateInfo *info);

	void renderWidget(std::vector<DfxColorID> colors);

	void renderShape(deepsg::SgNode *node, deepsg::SgStateInfo *info);
	void renderShapeColorOnly(deepsg::SgNode *node, deepsg::SgStateInfo *info);

	void renderInstances();

	void renderDelegates(deepsg::SgNode *node, deepsg::SgStateInfo *info);

	void renderPointCloud(deepsg::SgNode *node, deepsg::SgStateInfo *info);

	void renderVolume(deepsg::SgNode *node, deepsg::SgStateInfo *info);

	void drawSelectionMarquee();

	void initLighting();
public:

	DfxQViewportRenderGL_2_1();
	virtual ~DfxQViewportRenderGL_2_1();

	std::string name() override;
	ShadeMode shadingMode() override;
	void setShadingMode(ShadeMode) override;
	void init() override;
	void render() override;
	void drawCameraRenderRegion(int, int) override;
	void renderColors(ElementType) override;
	glm::ivec4 colorAtPosition(int, int, int) override;
	std::vector<DfxColorField> colorAtPosition(int x, int y, int w, int h, int canvasHeight) override;
	float depthAtPosition(int, int, int) override;
	void resize(int width, int height) override;
	
	// void renderWidgets() override;
	glm::mat4 getProjectionMatrix() override;
	glm::mat4 getViewMatrix() override;
	void setProjectionMatrix(glm::mat4) override;
	void setViewMatrix(glm::mat4) override;
	void setWorkGrid(DfxGrid2d) override;
	void setCameraPosition(glm::vec3) override;
	void setCameraPath(std::string) override;
	void setCameraTransform(glm::mat4) override;
	void drawWorkGrid() override;
	void setBackgroundColor(glm::vec3) override;
	glm::vec3 getBackgroundColor() override;
	
};