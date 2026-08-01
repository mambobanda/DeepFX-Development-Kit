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

#include <glm/vec3.hpp>

class DfxOpenVDBPointList {
private:
	struct OpenvdbParticle {
		openvdb::Vec3R p, v, c;
		openvdb::Real  r;
	};

	openvdb::Real   mRadiusScale;
	openvdb::Real   mVelocityScale;
	OpenvdbParticle **particles;
	int nbParticles;
	bool _useColorAttr;
public:
	typedef openvdb::Vec3R  PosType;

	DfxOpenVDBPointList();
	~DfxOpenVDBPointList();

	size_t  size()          const;

	void getPos(size_t n, openvdb::Vec3R& xyz) const;

	void getPosRad(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad) const;

	void getPosRadVel(size_t n, openvdb::Vec3R& xyz, openvdb::Real& rad, openvdb::Vec3R& vel) const;

	void getAtt(size_t n, openvdb::Vec3f& att) const;

	size_t addParticles(glm::vec3 *particles, glm::vec3 *velocities, glm::vec3 *colors, int *gids, int particleCount, float radius, int group = -1);

	void add(openvdb::Vec3R pos, openvdb::Vec3R vel, openvdb::Vec3R color, openvdb::Real radius);

	void setUseColorAttr(bool);
};