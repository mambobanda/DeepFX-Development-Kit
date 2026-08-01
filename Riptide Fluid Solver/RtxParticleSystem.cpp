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

#include "RtxParticleSystem.h"

#undef foreach
#include <openvdb/tools/MeshToVolume.h>

namespace rtx {

	ParticleSystem::ParticleSystem() {
		system = new SPHSystem;
		gravity = glm::vec3(0.f, -9.8f, 0.f);
		dt = 0.001f;
	}

	ParticleSystem::~ParticleSystem() {

	}

	void ParticleSystem::init(size_t size) {
		system->resize(size);
	}

	void ParticleSystem::addParticles(std::vector<glm::vec3> particles) {
		for (size_t inx = 0; inx < particles.size(); inx++) {
			system->positions()[inx] = particles[inx];
			system->type()[inx] = FLUID_PARTICLE;
			system->alive()[inx] = 1;
		}
	}

	void ParticleSystem::addVelocities(std::vector<glm::vec3> velocities) {
		for (size_t inx = 0; inx < velocities.size(); inx++) {
			system->velocity()[inx] = velocities[inx];
		}
	}

	SPHSystem *ParticleSystem::getSystem() {
		return system;
	}

	void ParticleSystem::integrate() {
		for (size_t inx = 0; inx < system->nbParticles(); inx++) {
			system->velocity_i()[inx] = system->positions()[inx];
			glm::vec3 vel_i = system->velocity()[inx] + (dt * gravity);
			glm::vec3 pos_i = system->positions()[inx]  + (vel_i * dt);

			if (!collisionTest(inx, system->positions()[inx], pos_i, system->velocity()[inx], vel_i)) {
				system->positions()[inx] = pos_i;
				system->velocity()[inx] = vel_i;
			}
		}
	}

	void ParticleSystem::collisionResponse00(size_t index, float distance, glm::vec3 normal, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 nextVel) {
		glm::vec3 delta = glm::normalize((nextPos - currentPos)) * (distance);
		system->positions()[index] = nextPos + delta;
		system->velocity()[index] = nextVel - 2.0f * (glm::dot(nextVel, normal) * normal);
	}

	void ParticleSystem::collisionResponse01(size_t index, float distance, glm::vec3 normal, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 currentVel, glm::vec3 nextVel) {
		
		float _frictionCoeffient = 0.3f;
		float restitutionCoefficient = 5.f;

		glm::vec3 delta = glm::normalize((nextPos - currentPos)) * (distance);

		glm::vec3 targetNormal = normal;
		glm::vec3 targetPoint = nextPos + delta;
		glm::vec3 colliderVelAtTargetPoint = currentVel;

		// Get new candidate relative velocity from the target point.
		glm::vec3 relativeVel = nextVel - colliderVelAtTargetPoint;
		float normalDotRelativeVel = glm::dot(targetNormal,relativeVel);
		glm::vec3 relativeVelN = normalDotRelativeVel * targetNormal;
		glm::vec3 relativeVelT = relativeVel - relativeVelN;

		// Check if the velocity is facing opposite direction of the surface
		// normal
		if (normalDotRelativeVel < 0.0) {
			// Apply restitution coefficient to the surface normal component of
			// the velocity
			glm::vec3 deltaRelativeVelN =
				(-restitutionCoefficient - 1.0f) * relativeVelN;
			relativeVelN *= -restitutionCoefficient;

			// Apply friction to the tangential component of the velocity
			// From Bridson et al., Robust Treatment of Collisions, Contact and
			// Friction for Cloth Animation, 2002
			// http://graphics.stanford.edu/papers/cloth-sig02/cloth.pdf
			auto rl = glm::length(relativeVelT);
			if (rl * rl > 0.0) {
				double frictionScale = std::max(
					1.0f - _frictionCoeffient * glm::length(deltaRelativeVelN) /
					relativeVelT.length(),
					0.0f);
				relativeVelT *= frictionScale;
			}

			// Reassemble the components
			system->velocity()[index] = relativeVelN + relativeVelT + colliderVelAtTargetPoint;
		}

		// Geometric fix
		system->positions()[index] = targetPoint;
	}

	bool ParticleSystem::collisionTest(size_t index, glm::vec3 currentPos, glm::vec3 nextPos, glm::vec3 currentVel, glm::vec3 nextVel) {

		for (auto grid : _collisionObjects) {

			auto point = openvdb::Vec3s( nextPos.x, nextPos.y, nextPos.z);

			auto point_index = (*grid)->worldToIndex(point);
			auto accessor = (*grid)->getConstAccessor();

			auto ijk = openvdb::Coord::floor(point_index);

			

				float wDistance = accessor.getValue(ijk);
				if (wDistance <= 0) {

					openvdb::Vec3s _normal(wDistance);
					ijk[0] += 1;
					_normal[0] += accessor.getValue(ijk);
					ijk[0] -= 1;
					ijk[1] += 1;
					_normal[1] += accessor.getValue(ijk);
					ijk[1] -= 1;
					ijk[2] += 1;
					_normal[2] += accessor.getValue(ijk);
					_normal.normalize();

					glm::vec3 normal(_normal.x(), _normal.y(), _normal.z());

					// collisionResponse00(index, wDistance, normal, currentPos, nextPos, nextVel);
					collisionResponse01(index, wDistance, normal, currentPos, nextPos, currentVel, nextVel);
					return true;
				}
			
			
		}

		return false;
	}

	void ParticleSystem::addCollisionObject(Mesh *mesh, float voxelSize) {

		auto tempMesh = extractMesh(mesh);

		openvdb::FloatGrid::Ptr *_ngrid = new  openvdb::FloatGrid::Ptr();

		std::vector<openvdb::Vec4I> quads;
		//openvdb::tools::QuadAndTriangleDataAdapter<openvdb::Vec3s, openvdb::Vec4I> tmsh(tempMesh.points, tempMesh.polys);

		openvdb::math::Transform::Ptr xform = openvdb::math::Transform::createLinearTransform(voxelSize);
		*_ngrid = openvdb::tools::meshToSignedDistanceField<openvdb::FloatGrid>(*xform, tempMesh.points, tempMesh.polys, quads, 3.f, 3.f);

		_collisionObjects.push_back(_ngrid);
	}

	void ParticleSystem::collisionTests() {
		for (auto grid : _collisionObjects) {

			auto point = openvdb::Vec3s(
				system->positions()[0].x,
				system->positions()[0].y,
				system->positions()[0].z);

			auto pInGridSpace = (*grid)->worldToIndex(point);
			auto accessor = (*grid)->getConstAccessor();

			auto ijk = openvdb::Coord::floor(pInGridSpace);

			if ((*grid)->tree().isValueOn(ijk)) {
				float wDistance = accessor.getValue(ijk);
				if (wDistance <= 0) {

					openvdb::Vec3s normal(wDistance);
					ijk[0] += 1;
					normal[0] += accessor.getValue(ijk);
					ijk[0] -= 1;
					ijk[1] += 1;
					normal[1] += accessor.getValue(ijk);
					ijk[1] -= 1;
					ijk[2] += 1;
					normal[2] += accessor.getValue(ijk);
					normal.normalize();

					glm::vec3 targetPoint = system->positions()[0] + wDistance * glm::vec3(normal.x(), normal.y(), normal.z());
					printf("1. Point is in or is on Mesh. Distance = %f. Normal < %f, %f, %f >. Target < %f, %f, %f >\n", wDistance, normal.x(), normal.y(), normal.z(), targetPoint.x, targetPoint.y, targetPoint.z);
				}
				else {
					printf("2. Point is NOT in or on Mesh. Distance = %f\n", wDistance);
				}
			}
			else {
				float wDistance = accessor.getValue(ijk);
				printf("3. Point is NOT in or on Mesh. Distance = %f\n", wDistance);

			}
		}
	}

	unsigned int ParticleSystem::VDB_TempMesh::polygonCount() {
		return polys.size();
	}

	ParticleSystem::VDB_TempMesh ParticleSystem::extractMesh(Mesh *mesh) {
		VDB_TempMesh tm;

		for (int i = 0; i < mesh->nbVerts; i++) {
			openvdb::Vec3s pos = openvdb::Vec3s(
				mesh->verts[i].x,
				mesh->verts[i].y,
				mesh->verts[i].z);
			tm.points.push_back(pos);
		}

		int counter = 0;

		if (mesh->nbFaces > 0) {
			for (int i = 0; i < mesh->nbFaces; i++) {
				tm.polys.push_back(openvdb::Vec3I(
					mesh->indices[counter],
					mesh->indices[counter + 1],
					mesh->indices[counter + 2]));

				counter += 3;
			}
		}

		return tm;
	}
}