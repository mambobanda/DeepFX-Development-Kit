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

#include <vector>
#include <unordered_map>
#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include <Dfx3dMeshData.h>

using namespace std;


namespace dfxcore {
	class VectorMeshSampler {
	private:
		typedef struct {
			glm::vec3 a;
			glm::vec3 b;
		}DirectEdge;

		static glm::vec3 lineIntersectionPoint(glm::vec3 line1_a, glm::vec3 line1_b, glm::vec3 line2_a, glm::vec3 line2_b);

		static vector<DirectEdge> findEdges(Dfx3dMeshData &mesh, glm::vec3 scale);
	private:
		static void sampleFaces(Dfx3dMeshData &mesh, glm::vec3 scale, float spacing, vector<glm::vec3>  &samples);
		static void sampleEdges(vector<DirectEdge> edges, float spacing, vector<glm::vec3>  &samples);
	public:
		static void sampleMesh(Dfx3dMeshData &mesh, glm::vec3 scale, float spacing, vector<glm::vec3> &positions);

	};
}