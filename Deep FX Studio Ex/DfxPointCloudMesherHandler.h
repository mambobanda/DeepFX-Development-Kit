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

#include "DfxPointCloudMesherHandler.h" 

#include <Fluent.h>
#include <SgRootNode.h>
#include "DfxFluentTypes.h"
#include "DfxQSceneManager.h"
#include "DfxBaseHandler.h"
#include "DfxPointCloudMesherNode.h"

class DfxPointCloudMesherHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxPointCloudMesherHandler();
	virtual ~DfxPointCloudMesherHandler();

	bool create();
	bool destroy();

	fl::Any *createModel(fl::Space *) override;
	DfxPointCloudMesherNode *mesherNode();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};
