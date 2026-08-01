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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxOpenVDBPointList.h"

DfxOpenVDBPointList::DfxOpenVDBPointList() {
	mRadiusScale = 1.0f;
	mVelocityScale = 1.0f;
	particles = NULL;
	_useColorAttr = false;
}

DfxOpenVDBPointList::~DfxOpenVDBPointList() {
	if (particles != NULL) {
		for (int i = 0; i < nbParticles; i++) {
			delete particles[i];
		}

		delete[] particles;
	}
}

void DfxOpenVDBPointList::add(openvdb::Vec3R pos, openvdb::Vec3R vel, openvdb::Vec3R color, openvdb::Real radius) {}

void DfxOpenVDBPointList::setUseColorAttr(bool bl) {
	_useColorAttr = bl;
}

size_t DfxOpenVDBPointList::addParticles(glm::vec3 *positions, glm::vec3 *velocities, glm::vec3 *colors, int *gids, int particleCount, float radius, int group) {

	size_t t_pcount = particleCount;

	if (group > -1 && gids != nullptr) {
		t_pcount = 0;
		for (int i = 0; i < particleCount; i++) {

			if (gids[i] != group) continue;

			t_pcount++;
		}
	}

	this->nbParticles = t_pcount;
	this->particles = new OpenvdbParticle*[t_pcount];

	size_t counter = 0;
	for (int i = 0; i < particleCount; i++) {

		if (group > -1 && gids != nullptr)
			if (gids[i] != group) continue;

		OpenvdbParticle *pa = new OpenvdbParticle();
		pa->p = openvdb::Vec3R((double)positions[i].x, (double)positions[i].y, (double)positions[i].z);
		pa->v = openvdb::Vec3R(0.f, 0.f, 0.f);
		pa->c = openvdb::Vec3R(0.f, 0.f, 0.f);
		//pa->v = openvdb::Vec3R((double)velocities[i].x, (double)velocities[i].y, (double)velocities[i].z);
		//pa->c = openvdb::Vec3R((double)colors[i].x, (double)colors[i].y, (double)colors[i].z);
		pa->r = radius;

		this->particles[counter] = pa;
		counter++;
	}

	return t_pcount;
}

size_t DfxOpenVDBPointList::size() const { return nbParticles; }

void DfxOpenVDBPointList::getPos(size_t n, openvdb::Vec3R&pos) const {
	pos = particles[n]->p;
}

void DfxOpenVDBPointList::getPosRad(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
}

void DfxOpenVDBPointList::getPosRadVel(size_t n, openvdb::Vec3R& pos, openvdb::Real& rad, openvdb::Vec3R& vel) const {
	pos = particles[n]->p;
	rad = mRadiusScale * particles[n]->r;
	vel = mVelocityScale * particles[n]->v;
}

void DfxOpenVDBPointList::getAtt(size_t n, openvdb::Vec3f& att) const {
	if (_useColorAttr)
		att = particles[n]->c;
	else
		att = particles[n]->v;
}