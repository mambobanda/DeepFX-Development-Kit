/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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
#include <SgGridAdapterInterface.h>
#include <QOpenGlFunctions_3_1>
#include <QOpenGLWidget>
#include <FlAny.h>
#include <DfxSpace.h>
#include <QOpenGLWidget.h>
#include "DfxQViewportRenderInterface.h"
#include "DfxMode.h"

class DfxVolumeCache : public QObject, public QOpenGLFunctions_3_1 {
	Q_OBJECT
private:

	static size_t ID_GEN;
	size_t _id;

	deepsg::SgGridAdapterInterface *_volumeData;
	fl::Any *_any;

	bool _ready;
	bool _cached;

private:
	bool _cache_active;

	// cube vertex array and vertex buffer object IDs
	GLuint cubeVBOID;
	GLuint cubeVAOID;
	GLuint cubeIndicesID;

	// Point Buffer
	GLuint voxelVAOID;
	GLuint voxelIntensityVBOID;
	GLuint voxelNormalVBOID;
	GLuint voxelCoordVBOID;

	// Texture ID
	GLuint densityTextureID;

	int textureSize;
	int XDIM;
	int YDIM;
	int ZDIM;

private:

	bool cacheDensityTexture();

	void applyLightingState(float voxPixSize, bool transparency = false);
	void undoLightingState();
public:

	DfxVolumeCache();
	virtual ~DfxVolumeCache();

	size_t id();

	deepsg::SgGridAdapterInterface *volumeData();
	void setVolumeData(deepsg::SgGridAdapterInterface *);

	fl::Any *any();
	void setAny(fl::Any *ay);

	void drawVolume(glm::mat4 mvp, glm::vec3 camPos);
	void drawVolume(float voxPixSize = 1.f, bool transparency = false);

	void clear();

	bool isCached();
	bool isReady();

	int DfxVolumeCache::index3d(int i, int j, int k, int size_i, int size_j, int size_k);
public slots:

	void cacheVolume2();
	void cacheVolume();
};