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

#include <vector>
#include <DsMesh.h>
#include <FlUtil.h>
#include "DfxDynaMesh.h"

class DfxSetTools {
public:
	static std::vector<std::string> allVertexSets(ds::Mesh *msh);
	static std::vector<std::string> allFaceSets(ds::Mesh *msh);
	static std::vector<std::string> allEdgeSets(ds::Mesh *msh);

	static std::vector<std::string> allFaceSets(DfxDynaMesh *msh);

};