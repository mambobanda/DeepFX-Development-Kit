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

#include "IndexedTriangleMesh.h"

void IndexedTriangleMesh::buildIndexedMesh(IndexedTriangleMesh::Triangle *input, uint32_t tri_count) {
	vertexArray.clear();
	indexArray.clear();

	for (size_t inx = 0; inx < tri_count; inx++) {

		unsigned rinx = 0;

		if (!findIndex(input[inx].a.x, input[inx].a.y, input[inx].a.z, rinx)) {
			vertexArray.push_back(input[inx].a);
			indexArray.push_back(vertexArray.size() - 1);
		}
		else {
			indexArray.push_back(rinx);
		}

		if (!findIndex(input[inx].b.x, input[inx].b.y, input[inx].b.z, rinx)) {
			vertexArray.push_back(input[inx].b);
			indexArray.push_back(vertexArray.size() - 1);
		}
		else {
			indexArray.push_back(rinx);
		}

		if (!findIndex(input[inx].c.x, input[inx].c.y, input[inx].c.z, rinx)) {
			vertexArray.push_back(input[inx].c);
			indexArray.push_back(vertexArray.size() - 1);
		}
		else {
			indexArray.push_back(rinx);
		}

	}
}

std::vector<uint32_t> &IndexedTriangleMesh::getIndexArray() {
	return indexArray;
}

std::vector<glm::vec3> &IndexedTriangleMesh::getVertexArray() {
	return vertexArray;
}

bool IndexedTriangleMesh::findIndex(float x, float y, float z, unsigned int & out_inx) {

	for (size_t inx = 0; inx < vertexArray.size(); inx++) {

		if (x == vertexArray[inx].x &&
			y == vertexArray[inx].y &&
			z == vertexArray[inx].z) {

			out_inx = inx;
			return true;
		}
	}

	return false;
}