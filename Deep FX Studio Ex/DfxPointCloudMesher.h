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

#undef Q_FOREACH
#include <openvdb/openvdb.h>
#define foreach Q_FOREACH

#include <vector>
#include <glm\vec3.hpp>
#include <Mesh.h>
#include "DfxPointCloudNode.h"
#include "DfxPointCloudMesherFilters.h"

inline void VDBInitialize() { openvdb::initialize(); }

class DfxPointCloudMesher {
private:
	float particleRadius;
	float halfWidth;
	int group;
public:

	DfxPointCloudMesher();
	virtual ~DfxPointCloudMesher();

	Mesh *meshParticles(DfxPointCloudNode *pcloud, float gridSize, float particleRadius, std::vector<DfxPointCloudMesherFilters::Filter*> filters = std::vector<DfxPointCloudMesherFilters::Filter*>());

	int   getGridRadius();
	float getParticleRadius();
	float getHalfWidth();
	void setParticleRadius(float);
	void setHalfWidth(float);
	void defaultSettings();
	void setGroup(int);
	int  getGroup();
};