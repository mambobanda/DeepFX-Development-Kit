/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2016 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#pragma once

#include "DfxVDBGlobal.h"

#ifdef USE_OPENVDB_LIB 

#define _HAS_STD_BYTE 0
#include <vector>
#include <glm\vec3.hpp>
#include "DfxVDBFilters.h"
#include "DfxVDBCommon.h"
#include "DfxTempIndexedMesh.h"


#undef foreach
#include <openvdb/openvdb.h>

using namespace std;

#define GRID_RADIUS_2_R     0
#define GRID_RADIUS_R       1
#define GRID_RADIUD_HALF_R  2

inline void VDBInitialize() { openvdb::initialize(); }

class GridSizes{
public:
  static inline float size(int v){
	 float temp[] = {0.04f, 0.02f, 0.01f};

	 return temp[v];
  }
};


class OpenvdbParticleSkinner{
private:
	float gridRadius;
	float particleRadius;
	float halfWidth;
public:
	OpenvdbParticleSkinner();
	
	void skinParticles(glm::vec3 *particles, int particleCount, Mesh *mesh, vector<vdb::Filter*> filters);
	//void addFilter(vdb::Filter *);
	//int getFilterCount();
	//vdb::Filter *getFilter(int);
	//void removeFilter(int);
	//void clearFilters();
	int getGridRadius();
	float getParticleRadius();
	float getHalfWidth();
	void setGridRadius(float );
	void setParticleRadius(float );
	void setHalfWidth(float );
	void defaultSettings();
};

#endif // USE_OPENVDB_LIB 