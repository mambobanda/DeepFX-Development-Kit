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

#include "DfxBulletShapeBuilder.h"

std::pair<btCollisionShape*, btTriangleMesh*> DfxBulletShapeBuilder::gimpactTirangleShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale) {
	
	if (gadapter != nullptr) {

		if (gadapter->getVertexCount() > 0 && gadapter->getPolygonCount() > 0) {

			auto mesh = new btTriangleMesh();

			for (size_t i = 0; i < gadapter->getPolygonCount(); i++) {
				auto rts = gadapter->polygonToTriangles(i);

				for (auto tri : rts) {

					auto a = gadapter->getVertex(tri.a) * scale;
					auto b = gadapter->getVertex(tri.b) * scale;
					auto c = gadapter->getVertex(tri.c) * scale;

					btVector3 vert0(a.x, a.y, a.z);
					btVector3 vert1(b.x, b.y, b.z);
					btVector3 vert2(c.x, c.y, c.z);

					mesh->addTriangle(vert0, vert1, vert2);
				}
			}

			auto _shape = new btGImpactMeshShape(mesh);
			_shape->updateBound();

			return std::pair<btCollisionShape*, btTriangleMesh*>(_shape, mesh);
		}
	}
	
	return std::pair<btCollisionShape*, btTriangleMesh*>(nullptr, nullptr);
}

std::pair<btCollisionShape*, btTriangleMesh*> DfxBulletShapeBuilder::bvhTirangleShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale) {

	if (gadapter != nullptr) {

		if (gadapter->getVertexCount() > 0 && gadapter->getPolygonCount() > 0) {

			auto mesh = new btTriangleMesh();

			for (size_t i = 0; i < gadapter->getPolygonCount(); i++) {
				auto rts = gadapter->polygonToTriangles(i);

				for (auto tri : rts) {

					auto a = gadapter->getVertex(tri.a) * scale;
					auto b = gadapter->getVertex(tri.b) * scale;
					auto c = gadapter->getVertex(tri.c) * scale;

					btVector3 vert0(a.x, a.y, a.z);
					btVector3 vert1(b.x, b.y, b.z);
					btVector3 vert2(c.x, c.y, c.z);

					mesh->addTriangle(vert0, vert1, vert2);
				}
			}

			auto _shape = new btBvhTriangleMeshShape(mesh, true);

			return std::pair<btCollisionShape*, btTriangleMesh*>(_shape, mesh);
		}
	}

	return std::pair<btCollisionShape*, btTriangleMesh*>(nullptr, nullptr);
}

btCollisionShape *DfxBulletShapeBuilder::convexHullShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale) {

	if (gadapter != nullptr) {

		if (gadapter != nullptr) {

			if (gadapter->getVertexCount() > 0) {
				auto _shape = new btConvexHullShape();
			
				for (size_t i = 0; i < gadapter->getVertexCount(); i++) {
					auto vert = gadapter->getVertex(i) * scale;
					((btConvexHullShape*)_shape)->addPoint(btVector3(vert.x, vert.y, vert.z));
				}

				//_shape->setLocalScaling(scl);
				_shape->optimizeConvexHull();
				_shape->initializePolyhedralFeatures();

				return _shape;
			}
		}
	}

	return nullptr;
}

btCollisionShape *DfxBulletShapeBuilder::boxShapeFromGeometry(deepsg::SgGeoNode *gnode) {

	if (gnode != nullptr) {
		auto gadapter = gnode->getGeometryAdapter();
		auto scale = gnode->scale();

		if (gadapter != nullptr) {
			DfxBounds bounds;
			bounds.empty();

			if (gadapter->getVertexCount() > 0) {

				for (size_t i = 0; i < gadapter->getVertexCount(); i++) {
					auto vert = gadapter->getVertex(i) * scale;
					bounds.add(vert.x, vert.y, vert.z);
				}

				auto a = std::abs((float)(bounds.xmax - bounds.xmin)) * 0.5f;
				auto b = std::abs((float)(bounds.ymax - bounds.ymin)) * 0.5f;
				auto c = std::abs((float)(bounds.zmax - bounds.zmin)) * 0.5f;

				auto _shape = new btBoxShape(btVector3(a, b, c));

				return _shape;
			}
		}
	}

	return nullptr;
}

btCollisionShape *DfxBulletShapeBuilder::sphereShapeFromGeometry(deepsg::SgGeoNode *gnode) {
	if (gnode != nullptr) {
		auto gadapter = gnode->getGeometryAdapter();
		auto scale = gnode->scale();

		if (gadapter != nullptr) {

			if (gadapter->getVertexCount() > 0) {
				auto temp = new btConvexHullShape();

				for (size_t i = 0; i < gadapter->getVertexCount(); i++) {
					auto vert = gadapter->getVertex(i) * scale;
					((btConvexHullShape*)temp)->addPoint(btVector3(vert.x, vert.y, vert.z));
				}

				btVector3 center;
				btScalar radius; 

				temp->getBoundingSphere(center, radius);
				delete temp;

				auto _shape = new btSphereShape(radius);

				return _shape;
			}
		}
	}

	return nullptr;
}