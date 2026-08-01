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

#include <DfxGlobal.h>
#include <FlModel.h>
#include "DfxBaseHandler.h"
#include "DfxFluentTypes.h"
#include "DfxPointReaderTool.h"

class DfxPointReaderHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxPointReaderHandler();
	virtual ~DfxPointReaderHandler();

	fl::Any *createModel(fl::Space *) override;

	bool create();
	deepsg::SgToolNode *createToolNode();
	bool destroy();

	deepsg::SgToolNode *toolNode();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};