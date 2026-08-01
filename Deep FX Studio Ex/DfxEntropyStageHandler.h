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

#include <FlModel.h>
#include <SgNode.h>
#include "DfxEntropyStageNode.h"
#include "DfxBaseHandler.h"
#include "DfxFluentTypes.h"

class DfxEntropyStageHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxEntropyStageHandler();
	virtual ~DfxEntropyStageHandler();

	fl::Any *createModel(fl::Space *) override;

	bool create();
	bool destroy();
	void destroyActorsProps();

	DfxEntropyStageNode *createStageNode();
	DfxEntropyStageNode *stageNode();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);

};