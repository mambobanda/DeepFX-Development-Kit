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

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcGeom/All.h>

class DfxAlembicMeshProxy {
public:
	Alembic::AbcGeom::OPolyMesh *mesh;
	Alembic::AbcGeom::OXform *xform;
	Alembic::AbcGeom::OPolyMeshSchema::Sample *sample;

	Alembic::Abc::float32_t *g_verts;
	Alembic::Abc::float32_t *g_normals;
	Alembic::Abc::float32_t *g_uvs;
	Alembic::Abc::int32_t *g_indices;
	Alembic::Abc::int32_t *g_counts;
};