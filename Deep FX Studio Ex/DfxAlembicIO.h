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

#include <Alembic/Abc/All.h>
#include <Alembic/AbcCoreAbstract/All.h>
#include <Alembic/AbcCoreOgawa/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCollection/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcMaterial/All.h>

#include <string>
#include <DfxUtil.h>
#include "DfxDynaMesh.h"
#include "DfxSurfaceDictionary.h"

namespace DfxAlembicIO {

	bool loadPolyMesh(Alembic::AbcGeom::IPolyMesh &pmesh, std::string objPath, unsigned int surfid_offset,
		DfxDynaMesh *mesh, int frame, float sample = 0, float scale = 1.f, int offset = 0, glm::dmat4 xform = glm::dmat4(1.0), bool loop = true, bool applyXform = false);
	bool findFirstPolyMesh(Alembic::AbcGeom::IPolyMesh &pmesh, Alembic::AbcGeom::IObject &obj, glm::dmat4 &xform, float sample, int offset);
	bool loadFirstPolyMesh(std::string path, std::string objPath, DfxDynaMesh *mesh=nullptr, float sample=0.f, float scale = 1.f, int offset = 0,  bool loop = true,
		bool applyXform = false);

	bool findLoadPolyMesh(std::string path, std::string name, Alembic::AbcGeom::IPolyMesh &pmesh, Alembic::AbcGeom::IObject *obj, 
		glm::dmat4 &xform = glm::dmat4(1.0), bool applyXform = false, std::string objPath = "", DfxDynaMesh *mesh = nullptr, float sample = 0.f, float scale = 0.f,
		int offset = 0, bool loop = true);

	void shapePathList(std::string path, std::vector<std::string> &shape_paths, Alembic::AbcGeom::IObject *obj);

	void accumulateXform(glm::dmat4 &xform, Alembic::AbcGeom::IXform &ixform_obj, int sample, int offset);
}