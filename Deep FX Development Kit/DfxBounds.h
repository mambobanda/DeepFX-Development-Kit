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

#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <limits>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\common.hpp>
#include "DfxRay.h"

#define ___X 0

#define ___Y 1

#define ___Z 2

#define ___CROSS(dest,v1,v2) \
			dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
			dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
			dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 



#define ___DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])



#define ___SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 



#define ___FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

inline int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])	// -NJMP-
{

	int q;

	float vmin[3], vmax[3], v;

	for (q = ___X; q <= ___Z; q++)

	{

		v = vert[q];					// -NJMP-

		if (normal[q]>0.0f)

		{

			vmin[q] = -maxbox[q] - v;	// -NJMP-

			vmax[q] = maxbox[q] - v;	// -NJMP-

		}

		else

		{

			vmin[q] = maxbox[q] - v;	// -NJMP-

			vmax[q] = -maxbox[q] - v;	// -NJMP-

		}

	}

	if (___DOT(normal, vmin)>0.0f) return 0;	// -NJMP-

	if (___DOT(normal, vmax) >= 0.0f) return 1;	// -NJMP-



	return 0;

}


/*======================== ___X-tests ========================*/

#define ___AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[___Y] - b*v0[___Z];			       	   \
	p2 = a*v2[___Y] - b*v2[___Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[___Y] + fb * boxhalfsize[___Z];   \
	if(min>rad || max<-rad) return 0;



#define ___AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[___Y] - b*v0[___Z];			           \
	p1 = a*v1[___Y] - b*v1[___Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[___Y] + fb * boxhalfsize[___Z];   \
	if(min>rad || max<-rad) return 0;



/*======================== ___Y-tests ========================*/

#define ___AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[___X] + b*v0[___Z];		      	   \
	p2 = -a*v2[___X] + b*v2[___Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[___X] + fb * boxhalfsize[___Z];   \
	if(min>rad || max<-rad) return 0;



#define ___AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[___X] + b*v0[___Z];		      	   \
	p1 = -a*v1[___X] + b*v1[___Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[___X] + fb * boxhalfsize[___Z];   \
	if(min>rad || max<-rad) return 0;



/*======================== ___Z-tests ========================*/



#define ___AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[___X] - b*v1[___Y];			           \
	p2 = a*v2[___X] - b*v2[___Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[___X] + fb * boxhalfsize[___Y];   \
	if(min>rad || max<-rad) return 0;



#define ___AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[___X] - b*v0[___Y];				   \
	p1 = a*v1[___X] - b*v1[___Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[___X] + fb * boxhalfsize[___Y];   \
	if(min>rad || max<-rad) return 0;



inline int triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3])
{



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

class DfxBounds {
public:
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;

	DfxBounds();

	DfxBounds(glm::vec3 min, glm::vec3 max);

	void empty();

	void add(float x, float y, float z);
	
	void add(float x, float y, float z, glm::mat4 trans);

	glm::vec3 center();

	glm::vec3 size();

	glm::vec3 radius();

	void addPadding(float x, float y, float z);

	bool intersectsPoint(float x, float y, float z);

	bool intersectsAABB(DfxBounds &b);

	glm::vec2 intersectsRay(const DfxRay& ray);

	int triangleBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3]);
};