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

#include <FlModel.h>
#include <SgNode.h>
#include "DfxQSceneManager.h"
#include "DfxBaseHandler.h"
#include "DfxFluentTypes.h"
#include "DfxRiptideCommon.h"

class DfxRiptideStageHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxRiptideStageHandler();
	virtual ~DfxRiptideStageHandler();

	fl::Any *createModel(fl::Space *) override;

	void destroyActorsProps();
	bool create();
	bool destroy();

	DfxRiptideStageNode *createStageNode();
	DfxRiptideStageNode *stageNode();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};