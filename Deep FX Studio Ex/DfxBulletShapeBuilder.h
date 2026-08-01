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

#include <DfxBounds.h>
#include <btBulletDynamicsCommon.h>
#include <SgGeoNode.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

class DfxBulletShapeBuilder {
public:
	static std::pair<btCollisionShape*, btTriangleMesh*> gimpactTirangleShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale);
	static std::pair<btCollisionShape*, btTriangleMesh*> bvhTirangleShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale);
	static btCollisionShape *convexHullShapeFromGeometry(deepsg::SgGeoAdapterInterface *gadapter, glm::vec3 scale);
	static btCollisionShape *boxShapeFromGeometry(deepsg::SgGeoNode *gnode);
	static btCollisionShape *sphereShapeFromGeometry(deepsg::SgGeoNode *gnode);
};