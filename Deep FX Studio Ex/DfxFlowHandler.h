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

#include <Fluent.h>
#include <FlModel.h>
#include <DfxGlobal.h>
#include <DfxBaseHandler.h>
#include <FlFlow.h>
#include "DfxFlowDB.h"
#include "DfxSpace.h"


class DfxFlowHandler: public DfxBaseHandler, public fl::Model {
private:
	static size_t DfxFlowHandler::ID_GEN;

public:
	static std::string DfxFlowHandler::model_id;

public:

	DfxFlowHandler();
	~DfxFlowHandler();

	fl::Flow *flow();

	fl::Any *createModel(fl::Space *) override;
	bool create(std::string label);
	bool destroy();

	static bool clearAllFlows();
	static std::vector<DfxFlowHandler> fetchAllFlowHandlers();
};