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

#include "DfxVDBGlobal.h"

#ifdef USE_OPENVDB_LIB 

#include <glm/vec3.hpp>

#undef foreach
#include <openvdb/openvdb.h>

class OpenvdbParticleList {
private:
	struct OpenvdbParticle {
		openvdb::Vec3R p, v;
		openvdb::Real  r;
	};

	openvdb::Real   mRadiusScale;
	openvdb::Real   mVelocityScale;
	OpenvdbParticle **particles;
	int nbParticles;

public:
	typedef openvdb::Vec3R  PosType;

	OpenvdbParticleList();
	~OpenvdbParticleList();

	size_t  size()          const;

	void getPos(size_t n, openvdb::Vec3R& xyz) const;

	void getPosRad(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad) const;

	void getPosRadVel(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad, openvdb::Vec3R& vel) const;

	void getAtt(size_t n, openvdb::Index32& att) const;

	void addParticles(glm::vec3 *particles, int particleCount, float radius);

	void addParticlesAndSizes(glm::vec3 *particle, int particleCount, float *radiusArray);

	openvdb::CoordBBox OpenvdbParticleList::getBBox(const openvdb::GridBase& grid);

	//void add(openvdb::Vec3R pos, openvdb::Vec3R vel, openvdb::Real radius);
};

#endif