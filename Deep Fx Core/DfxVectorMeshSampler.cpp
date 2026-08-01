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

#include "DfxVectorMeshSampler.h"

namespace dfxcore {

	glm::vec3 VectorMeshSampler::lineIntersectionPoint(glm::vec3 line1_a, glm::vec3 line1_b, glm::vec3 line2_a, glm::vec3 line2_b) {
		glm::vec3 a = line1_b - line1_a;
		glm::vec3 b = line2_b - line2_a;
		glm::vec3 c = line2_a - line1_a;

		glm::vec3 cross_ab = glm::cross(a, b);
		float length = glm::length(cross_ab);
		float cross_ab_length = length * length;
		glm::vec3 cross_cb = glm::cross(c, b);

		return line1_a + (glm::dot(cross_cb, cross_ab) / cross_ab_length) * a;
	}

	vector<VectorMeshSampler::DirectEdge> VectorMeshSampler::findEdges(Dfx3dMeshData &mesh, glm::vec3 scale) {

		// Utility Functions For unordered_map
		struct hashFunc {
			size_t operator()(const glm::vec3 &k) const {
				size_t h1 = std::hash<double>()(k.x);
				size_t h2 = std::hash<double>()(k.y);
				size_t h3 = std::hash<double>()(k.z);

				return (h1 ^ (h2 << 1)) ^ h3;
			}
		};

		struct equalsFunc {
			bool operator()(const glm::vec3 &lhs, const glm::vec3 &rhs)const {
				return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.y == rhs.z);
			}
		};

		// Find Edges
		vector<DirectEdge> edges;
		unordered_map< glm::vec3, DirectEdge, hashFunc, equalsFunc > hmap;

		int counter = 0;
		for (int i = 0; i < mesh.getIndexCount() / 3; i++) {

			glm::vec3 a = mesh.getVertex(mesh.getIndex(counter)) * scale;
			glm::vec3 b = mesh.getVertex(mesh.getIndex(counter + 1)) * scale;
			glm::vec3 c = mesh.getVertex(mesh.getIndex(counter + 2)) * scale;

			glm::vec3 ab_mid = (a + b) / 2.f;
			glm::vec3 ac_mid = (a + c) / 2.f;
			glm::vec3 bc_mid = (b + c) / 2.f;

			unordered_map< glm::vec3, DirectEdge, hashFunc, equalsFunc >::iterator ab_hash = hmap.find(ab_mid);
			unordered_map< glm::vec3, DirectEdge, hashFunc, equalsFunc >::iterator ac_hash = hmap.find(ac_mid);
			unordered_map< glm::vec3, DirectEdge, hashFunc, equalsFunc >::iterator bc_hash = hmap.find(bc_mid);

			if (ab_hash == hmap.end()) {
				DirectEdge de = { a, b };
				hmap[ab_mid] = de;
			}

			if (ac_hash == hmap.end()) {
				DirectEdge de = { a, c };
				hmap[ac_mid] = de;
			}

			if (bc_hash == hmap.end()) {
				DirectEdge de = { b, c };
				hmap[bc_mid] = de;
			}

			counter += 3;
		}

		unordered_map< glm::vec3, DirectEdge, hashFunc, equalsFunc >::iterator it = hmap.begin();
		for (; it != hmap.end(); it++) {
			edges.push_back(it->second);
		}

		return edges;
	}

	void VectorMeshSampler::sampleEdges(vector<DirectEdge> edges, float spacing, vector<glm::vec3>  &samples) {

		for (int i = 0; i < edges.size(); i++) {

			glm::vec3 ab = edges[i].a - edges[i].b;
			float ab_length = abs(glm::length(ab));
			float ab_edge_count = ab_length / spacing;
			glm::vec3 ab_edge_disp = ab / ab_edge_count;

			glm::vec3 disp_update = edges[i].a;
			samples.push_back(disp_update);
			for (int i = 0; i < (ab_edge_count - 1); i++) {
				disp_update -= ab_edge_disp;
				samples.push_back(disp_update);
			}
		}

	}

	void VectorMeshSampler::sampleFaces(Dfx3dMeshData &mesh, glm::vec3 scale, float spacing, vector<glm::vec3>  &samples) {

		int cnter = 0;

		for (int i = 0; i < mesh.getIndexCount() / 3; i++) {

			glm::vec3 a = mesh.getVertex(mesh.getIndex(cnter)) * scale;
			glm::vec3 b = mesh.getVertex(mesh.getIndex(cnter + 1)) * scale;
			glm::vec3 c = mesh.getVertex(mesh.getIndex(cnter + 2)) * scale;

			glm::vec3 ab = a - b;
			glm::vec3 bc = b - c;
			glm::vec3 ca = c - a;

			glm::vec3 line_vector(0.f);
			glm::vec3 line1_a(0.f);
			glm::vec3 line1_b(0.f);
			glm::vec3 line2_a(0.f);
			glm::vec3 line2_b(0.f);
			glm::vec3 line3_a(0.f);
			glm::vec3 line3_b(0.f);

			float ab_length = abs(glm::length(ab));
			float bc_length = abs(glm::length(bc));
			float ca_length = abs(glm::length(ca));

			glm::vec3 shortest_edge(0.f);
			glm::vec3 longest_edge(0.f);

			float ab_edge_count = ab_length / spacing;
			float bc_edge_count = bc_length / spacing;
			float ca_edge_count = ca_length / spacing;

			glm::vec3 ab_edge_disp = ab / ab_edge_count;
			glm::vec3 bc_edge_disp = bc / bc_edge_count;
			glm::vec3 ca_edge_disp = ca / ca_edge_count;

			if (ab_length < bc_length) {
				shortest_edge = ab;

				line1_a = a;
				line1_b = b;

				line2_a = c;
				line2_b = a;

				line3_a = c;
				line3_b = b;

				if (ca_length < ab_length) {
					shortest_edge = ca;
					line1_a = c;
					line1_b = a;

					line2_a = b;
					line2_b = a;

					line3_a = b;
					line3_b = c;
				}

			}
			else {
				shortest_edge = bc;
				line1_a = b;
				line1_b = c;


				line2_a = a;
				line2_b = b;

				line3_a = a;
				line3_b = c;

				if (ca_length < bc_length) {
					shortest_edge = ca;
					line1_a = c;
					line1_b = a;

					line2_a = b;
					line2_b = a;

					line3_a = b;
					line3_b = c;
				}
			}

			if (ab_length > bc_length) {
				longest_edge = ab;

				if (ca_length > ab_length)
					longest_edge = ca;
			}
			else {
				longest_edge = bc;

				if (ca_length > bc_length)
					longest_edge = ca;
			}

			glm::vec3 normal = glm::cross(shortest_edge, glm::cross(longest_edge, shortest_edge)) /
				glm::length(glm::cross(shortest_edge, glm::cross(longest_edge, shortest_edge)));

			float norm_length = glm::length(normal);
			float sweep_height = glm::dot(normal, longest_edge);

			float steps = sweep_height / spacing;

			line_vector = 10.f * shortest_edge;

			line1_a += line_vector;
			line1_b -= line_vector;

			bool sub = false;

			float signLength1 = glm::length(line1_a - line2_a);
			glm::vec3 tmp_line_a = line1_a + normal * (spacing);
			float signLength2 = glm::length(tmp_line_a - line2_a);

			if (signLength2 > signLength1) sub = true;

			for (int i = 1; i < (steps - 1); i++) {
				if (sub) {
					line1_a -= normal * (spacing);
					line1_b -= normal * (spacing);
				}
				else {
					line1_a += normal * (spacing);
					line1_b += normal * (spacing);
				}

				glm::vec3 i1 = lineIntersectionPoint(line1_a, line1_b, line2_a, line2_b);
				glm::vec3 i2 = lineIntersectionPoint(line1_a, line1_b, line3_a, line3_b);

				float count = glm::length(i1 - i2) / spacing;
				glm::vec3  disp = (i1 - i2) / count;

				glm::vec3 disp_update = i1 - disp;
				samples.push_back(disp_update);
				for (int j = 1; j < (count - 1); j++) {
					disp_update -= disp;
					samples.push_back(disp_update);
				}
			}

			cnter += 3;
		}
	}

	void VectorMeshSampler::sampleMesh(Dfx3dMeshData &mesh, glm::vec3 scale, float spacing, vector<glm::vec3> &positions) {
		vector<DirectEdge> edges = findEdges(mesh, scale);
		sampleEdges(edges, spacing, positions);
		sampleFaces(mesh, scale, spacing, positions);
	}
}