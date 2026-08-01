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

#include "DfxVDBCommon.h"

#ifdef USE_OPENVDB_LIB 

OpenvdbParticleList::OpenvdbParticleList() {
	mRadiusScale = 1.0f;
	mVelocityScale = 1.0f;
	particles = NULL;
}

OpenvdbParticleList::~OpenvdbParticleList() {
	if (particles != NULL) {
		for (int i = 0; i < nbParticles; i++) {
			delete particles[i];
		}

		delete[] particles;
	}
}

openvdb::CoordBBox OpenvdbParticleList::getBBox(const openvdb::GridBase& grid) {
	openvdb::CoordBBox bbox;
	openvdb::Coord &min = bbox.min(), &max = bbox.max();
	openvdb::Vec3R pos;
	openvdb::Real rad, invDx = 1 / grid.voxelSize()[0];
	for (size_t n = 0, e = this->size(); n < e; ++n) {
		this->getPosRad(n, pos, rad);
		const openvdb::Vec3d xyz = grid.worldToIndex(pos);
		const openvdb::Real   r = rad * invDx;
		for (int i = 0; i < 3; ++i) {
			min[i] = openvdb::math::Min(min[i], openvdb::math::Floor(xyz[i] - r));
			max[i] = openvdb::math::Max(max[i], openvdb::math::Ceil(xyz[i] + r));
		}
	}
	return bbox;
}

void OpenvdbParticleList::addParticles(glm::vec3 *particles, int particleCount, float radius) {

	this->nbParticles = particleCount;
	this->particles = new OpenvdbParticle*[particleCount];

	for (int i = 0; i < particleCount; i++) {

		OpenvdbParticle *pa = new OpenvdbParticle();
		pa->p = openvdb::Vec3R((double)particles[i].x, (double)particles[i].y, (double)particles[i].z);
		pa->v = openvdb::Vec3R((double)0.0, (double)0.0, (double)0.0);
		pa->r = radius;

		this->particles[i] = pa;
	}
}

void OpenvdbParticleList::addParticlesAndSizes(glm::vec3 *particles, int particleCount, float *radiusArray) {
	this->nbParticles = particleCount;
	this->particles = new OpenvdbParticle*[particleCount];

	for (int i = 0; i < particleCount; i++) {

		OpenvdbParticle *pa = new OpenvdbParticle();
		pa->p = openvdb::Vec3R((double)particles[i].x, (double)particles[i].y, (double)particles[i].z);
		pa->v = openvdb::Vec3R((double)0.0, (double)0.0, (double)0.0);
		pa->r = radiusArray[i];

		this->particles[i] = pa;
	}
}

size_t OpenvdbParticleList::size() const { return nbParticles; }

void OpenvdbParticleList::getPos(size_t n, openvdb::Vec3R&pos) const { pos = particles[n]->p; }

void OpenvdbParticleList::getPosRad(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
}

void OpenvdbParticleList::getPosRadVel(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad, openvdb::Vec3R& vel) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
	vel = mVelocityScale * particles[n]->v;
}


void OpenvdbParticleList::getAtt(size_t n, openvdb::Index32& att) const { att = openvdb::Index32(n); }

#endif