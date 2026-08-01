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

#include <DsMesh.h>
#include <Fluent.h>
#include <FlModel.h>
#include <SgRootNode.h>
#include <SgPointCloudNode.h>
#include "DfxPointCloudNode.h"
#include "DfxFluentTypes.h"
#include "DfxDeepSimplexGeoAdapter.h"
#include "DfxQSceneManager.h"
#include "DfxBaseHandler.h"
#include "DfxOpenGLCacheManager.h"
#include "DfxMeshAttributes.h"
#include "DfxRootAttribute.h"
#include "DfxAttributeTree.h"

class DfxPointCloudHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxPointCloudHandler();
	virtual ~DfxPointCloudHandler();

	fl::Any *createModel(fl::Space *) override;

	bool create();
	deepsg::SgPointCloudNode *createPointCloudNode();
	DfxAttribute *createAttributes();
	bool destroy();

	deepsg::SgPointCloudNode *pointCloudNode();
	ds::PointCloud *dsPointCloudData();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);

};