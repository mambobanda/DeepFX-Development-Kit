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
#include <openvdb/openvdb.h>
#include "RtxSPHSystem.h"
#include "RtxMesh.h"

namespace rtx {

	class ParticleSystem {
	private:

		typedef struct {
			std::vector<openvdb::Vec3s> points;
			//std::vector<openvdb::Vec4I> polys;
			std::vector<openvdb::Vec3I> polys;
			unsigned int polygonCount();
		}VDB_TempMesh;

	private:
		SPHSystem *system;
		glm::vec3 gravity;
		float dt;

	private:
		std::vector<openvdb::FloatGrid::Ptr*> _collisionObjects;

	private:
		VDB_TempMesh extractMesh(Mesh *mesh);

	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		void addParticles(std::vector<glm::vec3> particles);
		void addVelocities(std::vector<glm::vec3> velocities);

		void integrate();

		void init(size_t size);

		void addCollisionObject(Mesh *mesh, float voxelSize);
		void collisionResponse00(size_t index, float distance, glm::vec3 normal, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 nextVel);
		void collisionResponse01(size_t index, float distance, glm::vec3 normal, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 currentvel, glm::vec3 nextVel);
		bool collisionTest(size_t index, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 currentVell, glm::vec3 nextVel);
		void collisionTests();

		SPHSystem *getSystem();
	};
}