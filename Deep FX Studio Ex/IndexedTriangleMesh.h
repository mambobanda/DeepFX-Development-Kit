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
#include <glm/vec3.hpp>

class IndexedTriangleMesh {
public:
	class Triangle {
	public:
		glm::vec3 a;
		glm::vec3 b;
		glm::vec3 c;
	};
private:

	std::vector<uint32_t> indexArray;
	std::vector<glm::vec3> vertexArray;

	bool findIndex(float x, float y, float z, unsigned int & out_inx);

public:
	void buildIndexedMesh(IndexedTriangleMesh::Triangle *input, uint32_t tri_count);

	std::vector<uint32_t> &getIndexArray();

	std::vector<glm::vec3> &getVertexArray();
};
