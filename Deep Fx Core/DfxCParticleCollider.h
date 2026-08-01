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

#include <Dfx3dMeshData.h>
#include <glm/geometric.hpp>
#include "DfxCParticleCollider_btDbvtCallback.h"
#include "Util.h"

namespace dfxcore {

	template<typename T>
	class DfxCParticleCollider {
	private:
		Dfx3dMeshData *meshDataPtr;
		std::vector<DfxCParticleTriangle*> triangles;
		btDbvt *bvh;
		glm::mat4 trans;

		bool isCCWTriangle;

		T responseFunction;
	public:

		DfxCParticleCollider();
		~DfxCParticleCollider();

		void resolveCollision(float radius, float restitutionCoefficient, float _frictionCoeffient, glm::vec3 velocity, glm::vec3 currentPos, glm::vec3 lastPos, glm::vec3 &newPosition, glm::vec3 &newVelocity);

		void setMeshData(Dfx3dMeshData *);
		void setTransform(glm::mat4 trs);
		void initBVH();

		void setCCWTriangles(bool);

		void clear();

		bool isPenetrating(float radius, glm::vec3 current, glm::vec3 last, glm::vec3 &penetratingPoint, glm::vec3 &normal);
	};

	template<typename T>
	DfxCParticleCollider<T>::DfxCParticleCollider() {
		meshDataPtr = nullptr;
		bvh = nullptr;
		isCCWTriangle = true;
	}

	template<typename T>
	DfxCParticleCollider<T>::~DfxCParticleCollider() {

	}

	template<typename T>
	void DfxCParticleCollider<T>::resolveCollision(float radius, float restitutionCoefficient, float _frictionCoeffient, glm::vec3 velocity, glm::vec3 currentPos, glm::vec3 lastPos, glm::vec3 &newPosition, glm::vec3 &newVelocity) {

		glm::vec3 penetrationPoint(0);
		glm::vec3 normal(0);

		if (isPenetrating(radius, currentPos, lastPos, penetrationPoint, normal)) {

			responseFunction.response(restitutionCoefficient, _frictionCoeffient, radius, penetrationPoint, normal, velocity, newPosition, newVelocity);
		}
		else {
			newPosition = currentPos;
			newVelocity = velocity;
		}
	}

	template<typename T>
	void DfxCParticleCollider<T>::setMeshData(Dfx3dMeshData *mdata) {
		meshDataPtr = mdata;
	}

	template<typename T>
	void DfxCParticleCollider<T>::setTransform(glm::mat4 trs) {
		trans = trs;
	}


	template<typename T>
	void DfxCParticleCollider<T>::setCCWTriangles(bool val) {
		isCCWTriangle = val;
	}

	template<typename T>
	void DfxCParticleCollider<T>::initBVH() {

		if (meshDataPtr != nullptr) {
			bvh = new btDbvt;
			auto mesh = meshDataPtr;

			if (mesh->getVertexCount() > 3 && ((int)(mesh->getIndexCount() / 3)) >= 1) {

				for (size_t i = 0; i < mesh->getIndexCount() / 3; i++) {

					int counter = i * 3;

					glm::vec3 _a = transformPoint(trans, glm::vec3(
						mesh->getVertex(mesh->getIndex(counter)).x,
						mesh->getVertex(mesh->getIndex(counter)).y,
						mesh->getVertex(mesh->getIndex(counter)).z));

					glm::vec3 _b = transformPoint(trans, glm::vec3(
						mesh->getVertex(mesh->getIndex(counter + 1)).x,
						mesh->getVertex(mesh->getIndex(counter + 1)).y,
						mesh->getVertex(mesh->getIndex(counter + 1)).z));

					glm::vec3 _c = transformPoint(trans, glm::vec3(
						mesh->getVertex(mesh->getIndex(counter + 2)).x,
						mesh->getVertex(mesh->getIndex(counter + 2)).y,
						mesh->getVertex(mesh->getIndex(counter + 2)).z));

					btVector3 _btPoints[] =
					{ btVector3(_a.x, _a.y, _a.z),
					  btVector3(_b.x, _b.y, _b.z),
					  btVector3(_c.x, _c.y, _c.z) };

					DfxCParticleTriangle *t = new DfxCParticleTriangle;
					t->a = _a;
					t->b = _b;
					t->c = _c;


					btDbvtVolume volume = btDbvtVolume::FromPoints(_btPoints, 3);

					bvh->insert(volume, t);

					triangles.push_back(t);
				}
			}


		}
	}

	template<typename T>
	bool DfxCParticleCollider<T>::isPenetrating(float radius, glm::vec3 current, glm::vec3 last, glm::vec3 &penetratingPoint, glm::vec3 &normal) {
		bool success = false;

		if (bvh != nullptr) {
			glm::vec3 currentDisp = current + (glm::normalize(current - last) * radius);
			btVector3 r1(last.x, last.y, last.z);

			btVector3 r2(currentDisp.x, currentDisp.y, currentDisp.z);

			dfxcore::ParticleCollider_btDvtCallback collideCallback;
			collideCallback.currentPos = currentDisp;
			collideCallback.lastPos = last;
			collideCallback.ccw = isCCWTriangle;

			if (glm::distance(last, currentDisp) > 1.0e-6f) {
				bvh->rayTest(bvh->m_root, r1, r2, collideCallback);

				if (collideCallback.hasCollision) {
					penetratingPoint = collideCallback.penetrationPoint;
					normal = collideCallback.normal;
					success = true;
				}
			}
		}

		return success;
	}

	template<typename T>
	void DfxCParticleCollider<T>::clear() {
		delete bvh;
		bvh = nullptr;

		for (auto tri : triangles) {
			delete tri;
		}

		triangles.clear();

		meshDataPtr = nullptr;
	}
};
