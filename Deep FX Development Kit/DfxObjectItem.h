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

#include <experimental\filesystem>
#include <glm\geometric.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\normal.hpp>
#include "DfxObjectLoaderService.h"
#include "DfxViewItem.h"
#include "Dfx3dMeshData.h"
#include "DfxGlobal.h"
#include "DfxObjectDisplacementMap.h"

namespace filesystem = std::experimental::filesystem;

static DfxGlobalEntityName DfxObjectItem_Identifier = "2F2X252X_DFX_OBJECT_ITEM";

class DfxObjectItem : public DfxViewItem {
private:
	static DfxColorCounter color_id_counter;
private:
	GLuint vertexBufferID;
	GLuint indexBufferID;

	GLuint displayListID;
	GLuint displayListSelID;
	GLuint displayListSelEvtID;
	
	bool cached;
	bool ready;
	bool fileReferenced;

	DfxBounds itemBounds;

private:

	DfxObjectDisplacementMap *displacementMap;
	DfxObjectDisplacement *activeDisplacement;
public:

	static const DfxPropertyID mesh						= 0;

	static const DfxPropertyID outPosition				= 1;
	static const DfxPropertyID outRotation				= 2;
	static const DfxPropertyID outScale					= 3;
	static const DfxPropertyID inFilePath				= 4;
	static const DfxPropertyID inMode					= 5;
	static const DfxPropertyID inDisplacementIndex		= 6;
	static const DfxPropertyID inIgnoreLocalTransform	= 7;

	
public:

	DfxObjectItem(DfxGlobalEntityName uniquename);
	virtual ~DfxObjectItem();

	void drawMesh();

	void drawSilhouette();

	void draw() override;

	void drawSelection() override;

	void compute() override;

	DfxBounds bounds() override;

	bool selectable() override;

	void calculateBounds() override;

	void save(std::ofstream &) override;
	
	void load(std::ifstream &) override;

	virtual void copy(DfxEntity&) override;

	bool isRenderable() override;

	void bindOpenGlBuffers();

	void initOpenGlBuffer();

	static void initialize();
	
	Dfx3dMeshData *getMeshData();

	bool ignoreLocalTransform();

	bool renderDisplacement();

	bool isFileReferenced();

	void setFilerReferenced(bool);

	glm::vec3 getDisplacementVertex(size_t index);

	DfxObjectDisplacementMap *getDisplacementMap();

	static void *creator(DfxGlobalEntityName uniquename);

	bool isReady();

	void setIsReady(bool);

	std::string getFilePath();
	
	void setFilePath(std::string path);

	void syncTransforms();

	void cacheMesh();

	void deleteCache();
};