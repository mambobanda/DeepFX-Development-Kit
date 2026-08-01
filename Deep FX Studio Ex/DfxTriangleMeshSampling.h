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

#include <Mesh.h>
#include <vector>

class DfxTriangleMeshSampling {
private:

	static glm::vec3 lineIntersectionPoint(glm::vec3 line1_a, glm::vec3 line1_b, glm::vec3 line2_a, glm::vec3 line2_b);

	static std::vector<DirectEdge> findEdges(Mesh &mesh);

	static void sampleFaces(Mesh &mesh, float spacing, std::vector<glm::vec3>  &samples);
	static void sampleEdges(std::vector<DirectEdge> edges, float spacing, std::vector<glm::vec3>  &samples);
public:
	static void sampleMesh(Mesh &mesh, float spacing, std::vector<glm::vec3> &positions);

};