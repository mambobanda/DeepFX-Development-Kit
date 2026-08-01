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

#include <atomic>
#include <vector>
#include "DfxViewportCommon.h"

class DfxViewportSynchronizer {
private:

	static std::vector<DfxQSceneViewport*> _viewports;
	static std::atomic<int> counter;

public:

	static void addViewport(DfxQSceneViewport *vp);
	static void removeViewport(DfxQSceneViewport *vp);

	static void resetCounter();
	static void prepCounter();
	static void decrementCounter();

	static bool synchronized();
};