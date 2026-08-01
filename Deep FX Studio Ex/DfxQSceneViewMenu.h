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

#include <QApplication>
#include <QMenu>
#include <set>
#include "DfxGlobal.h"
#include "DfxMode.h"
#include "DfxMeshSelectionModel.h"
#include "DfxSetTools.h"
#include "DfxSurfaceDictionary.h"

class DfxQSceneViewMenu {
private:
	static QMenu *_currentMenu;

public:
	static void execObjectMode();
	static void execVertexMode();
	static void execEdgeMode();
	static void execFaceMode(); 
	static void exec();

	static void clear();
};