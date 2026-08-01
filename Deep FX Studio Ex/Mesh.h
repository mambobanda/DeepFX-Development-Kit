/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2016 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#pragma once

#include <glm/vec3.hpp>


class Mesh{
public:
	glm::vec3 *verts;
	glm::vec3 *normals;
	glm::vec2 *uvs;
	glm::vec3 *velocites;
	glm::vec3 *colors;
	int *materialID;
	unsigned int *indices;
	
	unsigned int nbVerts;
	unsigned int nbFaces;

	inline void clear(){
		if(verts != NULL)
			delete[] verts;

		if (materialID != NULL)
			delete[] materialID;

		if(normals!= NULL)
			delete[] normals;

		if(indices != NULL)
			delete[] indices;

		if (velocites != NULL)
			delete[] velocites;

		if (colors != NULL)
			delete[] colors;

		if (uvs != NULL)
			delete[] uvs;

		verts = NULL;
		normals = NULL;
		indices = NULL;
		velocites = NULL;
		materialID = NULL;
		colors = NULL;
		uvs = NULL;

		nbVerts = 0;
		nbFaces = 0;
	}

	inline void scale(glm::vec3 s){
		for(unsigned int i=0; i < nbVerts;  i++){
			verts[i] *= s;
		}
	}

	inline Mesh() {
		verts   = NULL;
		normals = NULL;
		indices = NULL;
		materialID = NULL;
		velocites = NULL;
		colors  = NULL;
		uvs = NULL;

		nbVerts = 0;
		nbFaces = 0;
	}
};

typedef struct{
	glm::vec3 a, b, c, d;
	glm::vec3 norm_a, norm_b, norm_c, norm_d;
}Quad;

typedef struct{
	glm::vec3 a, b, c;
	glm::vec3 norm_a, norm_b, norm_c;
}Tri;

typedef struct{
	unsigned int a;
	unsigned int b;
}Edge;

typedef struct{
	glm::vec3 a;
	glm::vec3 b;
}DirectEdge;

