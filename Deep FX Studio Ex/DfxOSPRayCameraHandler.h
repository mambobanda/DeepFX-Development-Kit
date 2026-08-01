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

#include <Fluent.h>
#include <SgRootNode.h>
#include <SgShapeNode.h>
#include "DfxFluentTypes.h"
#include "DfxQSceneManager.h"
#include "DfxBaseHandler.h"
#include "DfxOSPRayCameraWidget.h"
#include "DfxRenderTarget.h"

class DfxOSPRayCameraHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;
public:

	DfxOSPRayCameraHandler();
	virtual ~DfxOSPRayCameraHandler();

	bool create();
	bool destroy();

	fl::Any *createModel(fl::Space *) override;

	deepsg::SgShapeNode *createShapeNode();
	deepsg::SgShapeNode *shapeNode();

	int width();
	int height();

	static std::vector<DfxOSPRayCameraHandler> allCameras();
	static bool clearAllCameras();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};