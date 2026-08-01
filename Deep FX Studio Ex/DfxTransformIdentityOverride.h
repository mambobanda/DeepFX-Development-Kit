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

#include <SgTransformOverride.h>
#include "DfxTransformCache.h"

class DfxTransformIdentityOverride : public deepsg::SgTransformOverride {
private:
	DfxTransformCache *cachePtr;
	bool _ignoreInitialFrame;
public:
	DfxTransformIdentityOverride();
	virtual ~DfxTransformIdentityOverride();

	glm::mat4 evaluate(deepsg::SgTransformOverrideInfo info) override;

	bool ignoreInitialFrame();
	void setIgnoreInitialFrame(bool val);
};
