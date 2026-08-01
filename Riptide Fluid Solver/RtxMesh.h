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

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace rtx {

	class Mesh {
	public:

		glm::vec3 *verts;
		glm::vec3 *normals;
		glm::vec2 *uvs;
		glm::vec3 *velocites;
		glm::vec3 *colors;
		unsigned int *indices;

		unsigned int nbVerts;
		unsigned int nbFaces;

		inline void clear() {
			if (verts != nullptr)
				delete[] verts;

			if (normals != nullptr)
				delete[] normals;

			if (indices != nullptr)
				delete[] indices;

			if (velocites != nullptr)
				delete[] velocites;

			if (colors != nullptr)
				delete[] colors;

			if (uvs != nullptr)
				delete[] uvs;

			verts = nullptr;
			normals = nullptr;
			indices = nullptr;
			velocites = nullptr;
			colors = nullptr;
			uvs = nullptr;

			nbVerts = 0;
			nbFaces = 0;
		}

		inline void scale(glm::vec3 s) {
			for (unsigned int i = 0; i < nbVerts; i++) {
				verts[i] *= s;
			}
		}

		inline void transform(glm::mat4 xform) {
			for (unsigned int i = 0; i < nbVerts; i++) {
				verts[i] = glm::vec3(xform * glm::vec4(verts[i], 1.f));
			}
		}

		inline Mesh() {
			verts = nullptr;
			normals = nullptr;
			indices = nullptr;
			velocites = nullptr;
			colors = nullptr;
			uvs = nullptr;

			nbVerts = 0;
			nbFaces = 0;
		}
	};

	inline Mesh *cube() {

		Mesh *mesh = new Mesh;
		mesh->nbVerts = 8;
		mesh->nbFaces = 12;
		mesh->indices = new  unsigned int[3 * 12];
		mesh->verts = new glm::vec3[3 * 8];

		mesh->verts[0] = glm::vec3(-1.0f, -1.0f, 1.0f);
		mesh->verts[1] = glm::vec3(1.0f, -1.0f, 1.0f);
		mesh->verts[2] = glm::vec3(1.0f, 1.0f, 1.0f);
		mesh->verts[3] = glm::vec3(-1.0f, 1.0f, 1.0f);
			
		mesh->verts[4] = glm::vec3(-1.0f, -1.0f, -1.0f);
		mesh->verts[5] = glm::vec3(1.0f, -1.0f, -1.0f);
		mesh->verts[6] = glm::vec3(1.0f, 1.0f, -1.0f);
		mesh->verts[7] = glm::vec3(-1.0f, 1.0f, -1.0f);

		mesh->indices[0] = 0;
		mesh->indices[1] = 1;
		mesh->indices[2] = 2;

		mesh->indices[3] = 2;
		mesh->indices[4] = 3;
		mesh->indices[5] = 0;
		
		mesh->indices[6] = 1;
		mesh->indices[7] = 5;
		mesh->indices[8] = 6;
		
		mesh->indices[9]  = 6;
		mesh->indices[10] = 2;
		mesh->indices[11] = 1;

		mesh->indices[12] = 7;
		mesh->indices[13] = 6;
		mesh->indices[14] = 5;

		mesh->indices[15] = 5;
		mesh->indices[16] = 4;
		mesh->indices[17] = 7;

		mesh->indices[18] = 4;
		mesh->indices[19] = 0;
		mesh->indices[20] = 3;

		mesh->indices[21] = 3;
		mesh->indices[22] = 7;
		mesh->indices[23] = 4;

		mesh->indices[24] = 4;
		mesh->indices[25] = 5;
		mesh->indices[26] = 1;

		mesh->indices[27] = 1;
		mesh->indices[28] = 0;
		mesh->indices[29] = 4;

		mesh->indices[30] = 3;
		mesh->indices[31] = 2;
		mesh->indices[32] = 6;

		mesh->indices[33] = 6;
		mesh->indices[34] = 7;
		mesh->indices[35] = 3;

		return mesh;
	}
}