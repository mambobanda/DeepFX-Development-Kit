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

#pragma once

#include <DfxGlobal.h>
#include <DfxScene.h>
#include <DeepStudioGlobal.h>
#include <DfxLayerManager.h>

class DeepStudioShortcutFunctions {
public:
	static void activeViewportSolidMode();
	static void activeViewportWireframeMode();
	static void activeViewportHideSelected();
	static void activeViewportHideUnselected();
	static void activeViewportHideAll();
	static void activeViewportUnHideAll();
};