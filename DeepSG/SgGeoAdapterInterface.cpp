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

#include "SgGeoAdapterInterface.h"

namespace deepsg {

	SgGeoAdapterInterface::~SgGeoAdapterInterface() {

	}

	std::vector<SgGeoAdapterInterface::Triangle> SgGeoAdapterInterface::polygonToTriangles(size_t inx) {
		
		std::vector<SgGeoAdapterInterface::Triangle> output;
		std::unordered_map<size_t, size_t> originalVerts;
		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> newVerts;

		if (getPolygonDegree(inx) < 3) {
			return output;
		}
		else if (getPolygonDegree(inx) == 3) {
			SgGeoAdapterInterface::Triangle t;
			t.a = getPolygonVertexIndex(inx, 0);
			t.b = getPolygonVertexIndex(inx, 1);
			t.c = getPolygonVertexIndex(inx, 2);
			t.faceIndex = inx;

			output.push_back(t);
			return output;
		}
		else if (getPolygonDegree(inx) == 4) {

			glm::vec3 vec_a = getVertex(getPolygonVertexIndex(inx, 0));
			glm::vec3 vec_b = getVertex(getPolygonVertexIndex(inx, 1));
			glm::vec3 vec_c = getVertex(getPolygonVertexIndex(inx, 2));
			glm::vec3 vec_d = getVertex(getPolygonVertexIndex(inx, 3));

			float dist1 = glm::distance(vec_a, vec_c);
			float dist2 = glm::distance(vec_b, vec_d);

			SgGeoAdapterInterface::Triangle ta;
			SgGeoAdapterInterface::Triangle tb;

			ta.faceIndex = inx;
			tb.faceIndex = inx;

			if (dist1 > dist2) {
				ta.a = getPolygonVertexIndex(inx, 0);
				ta.b = getPolygonVertexIndex(inx, 1);
				ta.c = getPolygonVertexIndex(inx, 3);

				tb.a = getPolygonVertexIndex(inx, 1);
				tb.b = getPolygonVertexIndex(inx, 2);
				tb.c = getPolygonVertexIndex(inx, 3);
			}
			else {
				ta.a = getPolygonVertexIndex(inx, 0);
				ta.b = getPolygonVertexIndex(inx, 1);
				ta.c = getPolygonVertexIndex(inx, 2);

				tb.a = getPolygonVertexIndex(inx, 0);
				tb.b = getPolygonVertexIndex(inx, 2);
				tb.c = getPolygonVertexIndex(inx, 3);
			}

			output.push_back(ta);
			output.push_back(tb);

			return output;
		}

		for (int i = 0; i < getPolygonDegree(inx); i++) {
			size_t index = getPolygonVertexIndex(inx, i);
			verts.push_back(getVertex(index));
		}	

		ds::Tesselator tess;
		tess.init();
		tess.newPoly(verts.size());

		for (int i = 0; i < verts.size(); i++) {
			glm::vec3 v3 = verts[i];
			tess.addVertex(i, v3.x, v3.y, v3.z);
		}

		tess.commitPoly();
		tess.tesselate();

		for (int i = 0; i < verts.size(); i++) {
			size_t current = 3 * i;
			glm::vec3 v(tess.vertValue(current), tess.vertValue(current + 1), tess.vertValue(current + 2));
			newVerts.push_back(v);
		}

		for (size_t i = 0; i < newVerts.size(); i++) {
			for (int j = 0; j < verts.size(); j++) {
				if (newVerts[i] == verts[j]) {

					std::pair<size_t, size_t> p(i, getPolygonVertexIndex(inx, j));
					originalVerts.insert(p);
				}
			}
		}

		if (originalVerts.size() == verts.size()) {
			for (int i = 0; i < tess.numberOfTriangles(); i++) {
				SgGeoAdapterInterface::Triangle tri;

				size_t current = 3 * i;
				int inx00 = tess.faceIndex(current);
				int inx01 = tess.faceIndex(current + 1);
				int inx02 = tess.faceIndex(current + 2);

				tri.a = originalVerts.find(inx00)->second;
				tri.b = originalVerts.find(inx01)->second;
				tri.c = originalVerts.find(inx02)->second;

				tri.faceIndex = inx;

				output.push_back(tri);
			}
		}
		
		return output;
	}

	std::vector<SgGeoAdapterInterface::Triangle> SgGeoAdapterInterface::polygonToTriangles(glm::vec3 *inverts, int degree, std::vector<size_t> &indices, size_t faceIndex) {

		std::vector<SgGeoAdapterInterface::Triangle> output;
		std::unordered_map<size_t, size_t> originalVerts;
		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> newVerts;

		if (degree < 3) {
			return output;
		}
		else if (degree == 3) {
			SgGeoAdapterInterface::Triangle t;
			t.a = indices[0];
			t.b = indices[1];
			t.c = indices[2];
			t.faceIndex = faceIndex;

			output.push_back(t);
			return output;
		}
		else if (degree == 4) {

			glm::vec3 vec_a = inverts[indices[0]];
			glm::vec3 vec_b = inverts[indices[1]];
			glm::vec3 vec_c = inverts[indices[2]];
			glm::vec3 vec_d = inverts[indices[3]];

			float dist1 = glm::distance(vec_a, vec_c);
			float dist2 = glm::distance(vec_b, vec_d);

			SgGeoAdapterInterface::Triangle ta;
			SgGeoAdapterInterface::Triangle tb;

			ta.faceIndex = faceIndex;
			tb.faceIndex = faceIndex;

			if (dist1 > dist2) {
				ta.a = indices[0];
				ta.b = indices[1];
				ta.c = indices[3];

				tb.a = indices[1];
				tb.b = indices[2];
				tb.c = indices[3];
			}
			else {
				ta.a = indices[0];
				ta.b = indices[1];
				ta.c = indices[2];

				tb.a = indices[0];
				tb.b = indices[2];
				tb.c = indices[3];
			}

			output.push_back(ta);
			output.push_back(tb);

			return output;
		}

		for (int i = 0; i < degree; i++) {
			size_t index = indices[i];
			verts.push_back(inverts[index]);
		}

		ds::Tesselator tess;
		tess.init();
		tess.newPoly(verts.size());

		for (int i = 0; i < verts.size(); i++) {
			glm::vec3 v3 = verts[i];
			tess.addVertex(i, v3.x, v3.y, v3.z);
		}

		tess.commitPoly();
		tess.tesselate();

		for (int i = 0; i < verts.size(); i++) {
			size_t current = 3 * i;
			glm::vec3 v(tess.vertValue(current), tess.vertValue(current + 1), tess.vertValue(current + 2));
			newVerts.push_back(v);
		}

		for (size_t i = 0; i < newVerts.size(); i++) {
			for (int j = 0; j < verts.size(); j++) {
				if (newVerts[i] == verts[j]) {

					std::pair<size_t, size_t> p(i, indices[j]);
					originalVerts.insert(p);
				}
			}
		}

		if (originalVerts.size() == verts.size()) {
			for (int i = 0; i < tess.numberOfTriangles(); i++) {
				SgGeoAdapterInterface::Triangle tri;

				size_t current = 3 * i;
				int inx00 = tess.faceIndex(current);
				int inx01 = tess.faceIndex(current + 1);
				int inx02 = tess.faceIndex(current + 2);

				tri.a = originalVerts.find(inx00)->second;
				tri.b = originalVerts.find(inx01)->second;
				tri.c = originalVerts.find(inx02)->second;

				tri.faceIndex = faceIndex;

				output.push_back(tri);
			}
		}

		return output;
	}

	std::vector<SgGeoAdapterInterface::Triangle> SgGeoAdapterInterface::polygonToTriangles(float *inverts, int degree, std::vector<size_t> &indices, size_t faceIndex) {
		std::vector<SgGeoAdapterInterface::Triangle> output;
		std::unordered_map<size_t, size_t> originalVerts;
		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> newVerts;

		if (degree < 3) {
			return output;
		}
		else if (degree == 3) {
			SgGeoAdapterInterface::Triangle t;
			t.a = indices[0];
			t.b = indices[1];
			t.c = indices[2];
			t.faceIndex = faceIndex;

			output.push_back(t);
			return output;
		}
		else if (degree == 4) {

			size_t ia = indices[0];
			size_t ib = indices[1];
			size_t ic = indices[2];
			size_t id = indices[3];

			glm::vec3 vec_a = glm::vec3(inverts[ia], inverts[ia + 1], inverts[ia + 2]);
			glm::vec3 vec_b = glm::vec3(inverts[ib], inverts[ib + 1], inverts[ib + 2]);
			glm::vec3 vec_c = glm::vec3(inverts[ic], inverts[ic + 1], inverts[ic + 2]);
			glm::vec3 vec_d = glm::vec3(inverts[id], inverts[id + 1], inverts[id + 2]);

			float dist1 = glm::distance(vec_a, vec_c);
			float dist2 = glm::distance(vec_b, vec_d);

			SgGeoAdapterInterface::Triangle ta;
			SgGeoAdapterInterface::Triangle tb;

			ta.faceIndex = faceIndex;
			tb.faceIndex = faceIndex;

			if (dist1 > dist2) {
				ta.a = indices[0];
				ta.b = indices[1];
				ta.c = indices[3];

				tb.a = indices[1];
				tb.b = indices[2];
				tb.c = indices[3];
			}
			else {
				ta.a = indices[0];
				ta.b = indices[1];
				ta.c = indices[2];

				tb.a = indices[0];
				tb.b = indices[2];
				tb.c = indices[3];
			}

			output.push_back(ta);
			output.push_back(tb);

			return output;
		}

		for (int i = 0; i < degree; i++) {
			size_t index = indices[i];
			auto offset = index * 3;
			glm::vec3 vert(inverts[offset], inverts[offset + 1], inverts[offset + 2]);
			verts.push_back(vert);
		}

		ds::Tesselator tess;
		tess.init();
		tess.newPoly(verts.size());

		for (int i = 0; i < verts.size(); i++) {
			glm::vec3 v3 = verts[i];
			tess.addVertex(i, v3.x, v3.y, v3.z);
		}

		tess.commitPoly();
		tess.tesselate();

		for (int i = 0; i < verts.size(); i++) {
			size_t current = 3 * i;
			glm::vec3 v(tess.vertValue(current), tess.vertValue(current + 1), tess.vertValue(current + 2));
			newVerts.push_back(v);
		}

		for (size_t i = 0; i < newVerts.size(); i++) {
			for (int j = 0; j < verts.size(); j++) {
				if (newVerts[i] == verts[j]) {

					std::pair<size_t, size_t> p(i, indices[j]);
					originalVerts.insert(p);
				}
			}
		}

		if (originalVerts.size() == verts.size()) {
			for (int i = 0; i < tess.numberOfTriangles(); i++) {
				SgGeoAdapterInterface::Triangle tri;

				size_t current = 3 * i;
				int inx00 = tess.faceIndex(current);
				int inx01 = tess.faceIndex(current + 1);
				int inx02 = tess.faceIndex(current + 2);

				tri.a = originalVerts.find(inx00)->second;
				tri.b = originalVerts.find(inx01)->second;
				tri.c = originalVerts.find(inx02)->second;

				tri.faceIndex = faceIndex;

				output.push_back(tri);
			}
		}

		return output;
	}

	std::vector<glm::vec3> SgGeoAdapterInterface::calculateSmoothNormals(float *verts) {
		std::vector<glm::vec3> output;

		auto triangleCount = getTriangleIndexListSize() / 3;
		auto _indices = getTriangleIndexList();

		if (getVertexCount() > 0)  output.resize(getVertexCount());

		for (int i = 0; i < triangleCount; i++) {

			int i_of = i * 3;
			

			glm::vec3 a, b, c;

			if (verts != nullptr) {
				int v1_of = _indices[i_of] * 3;
				int v2_of = _indices[i_of + 1] * 3;
				int v3_of = _indices[i_of + 2] * 3;

				a = glm::vec3(verts[v1_of], verts[v1_of + 1], verts[v1_of + 2]);
				b = glm::vec3(verts[v2_of], verts[v2_of + 1], verts[v2_of + 2]);
				c = glm::vec3(verts[v3_of], verts[v3_of + 1], verts[v3_of + 2]);
			}
			else {
				a = getVertex(_indices[i_of]);
				b = getVertex(_indices[i_of + 1]);
				c = getVertex(_indices[i_of + 2]);
			}

			glm::vec3 u(b - a);
			glm::vec3 v(c - a);

			glm::vec3 fnorm = glm::cross(u, v);
			// glm::vec3 normi = glm::normalize(fnorm);

			output[_indices[i_of]] += fnorm;
			output[_indices[i_of + 1]] += fnorm;
			output[_indices[i_of + 2]] += fnorm;

		}

		for (int i = 0; i < getVertexCount(); i++) {
			int i_of = i * 3;	
			output[i] = glm::normalize(output[i]);
		}

		return output;
	}

}