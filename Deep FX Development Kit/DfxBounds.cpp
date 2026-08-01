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
#include "DfxBounds.h"

DfxBounds::DfxBounds() {
	empty();
}

DfxBounds::DfxBounds(glm::vec3 min, glm::vec3 max) {
	xmin = min.x; ymin = min.y; zmin = min.z;
	xmax = max.x; ymax = max.y; zmax = max.z;
}

void DfxBounds::empty() {
	xmin = ymin = zmin = std::numeric_limits<float>::infinity();
	xmax = ymax = zmax = -std::numeric_limits<float>::infinity();
}

void DfxBounds::add(float x, float y, float z) {
	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	if (z < zmin) zmin = z;
	if (z > zmax) zmax = z;
}

void DfxBounds::add(float x, float y, float z, glm::mat4 trans) {

	glm::vec3 tempPoint(x,y,z);

	tempPoint = glm::vec3(trans * glm::vec4(tempPoint, 1.f));
	x = tempPoint.x;
	y = tempPoint.y;
	z = tempPoint.z;

	if (x < xmin) xmin = x;
	if (x > xmax) xmax = x;
	if (y < ymin) ymin = y;
	if (y > ymax) ymax = y;
	if (z < zmin) zmin = z;
	if (z > zmax) zmax = z;
}

glm::vec3 DfxBounds::center() {
	glm::vec3 ct;
	ct.x = (xmin + xmax)*0.5f;
	ct.y = (ymin + ymax)*0.5f;
	ct.z = (zmin + zmax)*0.5f;

	return ct;
}

glm::vec3 DfxBounds::size() {
	glm::vec3 ct;
	ct.x = (xmax - xmin);
	ct.y = (ymax - ymin);
	ct.z = (zmax - zmin);

	return ct;
}

glm::vec3 DfxBounds::radius() {
	glm::vec3 ct = size();
	ct.x *= 0.5;
	ct.y *= 0.5;
	ct.z *= 0.5;

	return ct;
}

void DfxBounds::addPadding(float x, float y, float z) {
	xmin -= x;
	ymin -= y;
	zmin -= z;
	xmax += x;
	ymax += y;
	zmax += z;
}

bool DfxBounds::intersectsPoint(float x, float y, float z) {
	if (x < xmin)
		return false;

	if (y < ymin)
		return false;

	if (z < zmin)
		return false;

	if (x > xmax)
		return false;

	if (y > ymax)
		return false;

	if (z > zmax)
		return false;

	return true;
}

bool DfxBounds::intersectsAABB(DfxBounds &b) {
	if (xmax < b.xmin) return false;
	if (xmin > b.xmax) return false;
	if (ymax < b.ymin) return false;
	if (ymin > b.ymax) return false;
	if (zmax < b.zmin) return false;
	if (zmin > b.zmax) return false;

	return true;
}

glm::vec2 DfxBounds::intersectsRay(const DfxRay& ray) {

	glm::vec3 inv_dir = 1.0f / ray.direction;
	glm::vec3 tMin = (glm::vec3(xmin, ymin, zmin) - ray.origin) * inv_dir;
	glm::vec3 tMax = (glm::vec3(xmax, ymax, zmax) - ray.origin) * inv_dir;
	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm::max(tMin, tMax);
	float tNear = std::max(std::max(t1.x, t1.y), t1.z);
	float tFar = std::min(std::min(t2.x, t2.y), t2.z);

	return glm::vec2(tNear, tFar);
}

int DfxBounds::triangleBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3]) {
	/*    use separating axis theorem to test overlap between triangle and box */

	/*    need to test for overlap in these directions: */

	/*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */

	/*       we do not even need to test these) */

	/*    2) normal of the triangle */

	/*    3) crossproduct(edge from tri, {x,y,z}-directin) */

	/*       this gives 3x3=9 more tests */

	float v0[3], v1[3], v2[3];

	//   float axis[3];

	float min, max, p0, p1, p2, rad, fex, fey, fez;		// -NJMP- "d" local variable removed

	float normal[3], e0[3], e1[3], e2[3];



	/* This is the fastest branch on Sun */

	/* move everything so that the boxcenter is in (0,0,0) */

	___SUB(v0, triverts[0], boxcenter);

	___SUB(v1, triverts[1], boxcenter);

	___SUB(v2, triverts[2], boxcenter);



	/* compute triangle edges */

	___SUB(e0, v1, v0);      /* tri edge 0 */

	___SUB(e1, v2, v1);      /* tri edge 1 */

	___SUB(e2, v0, v2);      /* tri edge 2 */



							 /* Bullet 3:  */

							 /*  test the 9 tests first (this was faster) */

	fex = fabsf(e0[___X]);

	fey = fabsf(e0[___Y]);

	fez = fabsf(e0[___Z]);

	___AXISTEST_X01(e0[___Z], e0[___Y], fez, fey);

	___AXISTEST_Y02(e0[___Z], e0[___X], fez, fex);

	___AXISTEST_Z12(e0[___Y], e0[___X], fey, fex);


	fex = fabsf(e1[___X]);

	fey = fabsf(e1[___Y]);

	fez = fabsf(e1[___Z]);


	___AXISTEST_X01(e1[___Z], e1[___Y], fez, fey);

	___AXISTEST_Y02(e1[___Z], e1[___X], fez, fex);

	___AXISTEST_Z0(e1[___Y], e1[___X], fey, fex);


	fex = fabsf(e2[___X]);

	fey = fabsf(e2[___Y]);

	fez = fabsf(e2[___Z]);

	___AXISTEST_X2(e2[___Z], e2[___Y], fez, fey);

	___AXISTEST_Y1(e2[___Z], e2[___X], fez, fex);

	___AXISTEST_Z12(e2[___Y], e2[___X], fey, fex);



	/* Bullet 1: */

	/*  first test overlap in the {x,y,z}-directions */

	/*  find min, max of the triangle each direction, and test for overlap in */

	/*  that direction -- this is equivalent to testing a minimal AABB around */

	/*  the triangle against the AABB */



	/* test in ___X-direction */

	___FINDMINMAX(v0[___X], v1[___X], v2[___X], min, max);

	if (min>boxhalfsize[___X] || max<-boxhalfsize[___X]) return 0;



	/* test in ___Y-direction */

	___FINDMINMAX(v0[___Y], v1[___Y], v2[___Y], min, max);

	if (min>boxhalfsize[___Y] || max<-boxhalfsize[___Y]) return 0;



	/* test in ___Z-direction */

	___FINDMINMAX(v0[___Z], v1[___Z], v2[___Z], min, max);

	if (min>boxhalfsize[___Z] || max<-boxhalfsize[___Z]) return 0;



	/* Bullet 2: */

	/*  test if the box intersects the plane of the triangle */

	/*  compute plane equation of triangle: normal*x+d=0 */

	___CROSS(normal, e0, e1);

	// -NJMP- (line removed here)

	if (!planeBoxOverlap(normal, v0, boxhalfsize)) return 0;	// -NJMP-



	return 1;   /* box and triangle overlaps */
}