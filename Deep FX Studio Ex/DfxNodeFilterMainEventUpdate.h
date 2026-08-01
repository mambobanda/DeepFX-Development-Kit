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

#include <SgUtil.h>
#include <SgRootNode.h>
#include "DfxMainEventCommon.h"
#include "DfxMainEventProcess.h"
#include "DfxMainEventUpdateID.h"
#include "DfxMode.h"


class DfxNodeFilterMainEventUpdate : public DfxMainEventProcess {
public:
	DfxNodeFilterMainEventUpdate();
	virtual ~DfxNodeFilterMainEventUpdate();

	void process(int frame, unsigned int updateType) override;
};