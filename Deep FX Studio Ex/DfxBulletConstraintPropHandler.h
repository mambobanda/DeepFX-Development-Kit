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

#include <SgStageNode.h>
#include <FlModel.h>
#include "DfxBaseHandler.h"
#include "DfxBulletCommon.h"
#include "DfxFluentTypes.h"

class DfxBulletConstraintPropHandler : public DfxBaseHandler, public fl::Model {
private:
	static size_t ID_GEN;

public:
	static std::string model_id;

public:

	DfxBulletConstraintPropHandler();
	virtual ~DfxBulletConstraintPropHandler();

	fl::Any *createModel(fl::Space *) override;

	bool create();
	bool destroy();

	DfxBulletConstraintPropNode *createPropNode(deepsg::SgStageNode *stage);
	DfxBulletConstraintPropNode *propNode();

	static deepsg::SgNode* loadNode(std::string referencePath, deepsg::SgNode *parent);
	static void deleteNode(deepsg::SgNode *node);
};