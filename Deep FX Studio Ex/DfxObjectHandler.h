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

#include <DsMesh.h>
#include <Fluent.h>
#include <SgRootNode.h>
#include <DfxResource.h>
#include <DfxDynaMesh.h>
#include "DfxOpenGLCacheQueue.h"
#include "DfxGeometryNode.h"
#include "DfxFluentTypes.h"
#include "DfxDeepSimplexGeoAdapter.h"
#include "DfxQSceneManager.h"
#include "DfxBaseHandler.h"
#include "DfxOpenGLCacheManager.h"
#include "DfxMeshAttributes.h"
#include "DfxRootAttribute.h"
#include "DfxAttributeTree.h"
#include "DfxResourcePtr.h"
#include "DfxMaterialLinker.h"
#include "DfxSurfaceDictionary.h"

class DfxObjectHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxObjectHandler();
	virtual ~DfxObjectHandler();

	bool create(DfxResourcePtr *msh, std::string original_path = "**", std::string name = "");
	deepsg::SgGeoNode *createGeoNode();
	DfxAttribute *createAttributes();
	bool destroy();

	deepsg::SgGeoNode *geoNode();
	DfxResourcePtr *dsMeshData();
	DfxObjectCache *objectCache();
	DfxObjectCache *fetchObjectCache();
	DfxAttribute *attributes();

	bool updateCache();
	void setUpdateCache(bool val);

	bool useSmoothShading();

	fl::Any *createModel(fl::Space *) override;

	static bool clearAllObjects();

	static void releaseDeepSimplexResource(void *ptr);

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};