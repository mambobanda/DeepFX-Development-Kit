/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once


#include "ExportDef.h"
#include "DfxVDBFilter.h"
#include "DfxVDBOutputMesh.h"

extern "C"
{ 

	DfxVDBOutputMesh DFX_EXPORT *skinParticles(DfxVDBFloat3 *in_points, unsigned int pointCount, float gridRadius, float particleRadius, float halfWidth, VDBFilter *filters, unsigned int filterCount, DfxVDBOutputMeshFunctions *meshFunc);
}