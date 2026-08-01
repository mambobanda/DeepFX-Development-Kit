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

#include "SamplingTriangleMesh.h"

namespace SPH {

	xTriangleMesh::xTriangleMesh()
	{
	}

	xTriangleMesh::~xTriangleMesh()
	{
		release();
	}

	void xTriangleMesh::initMesh(const unsigned int nPoints, const unsigned int nFaces)
	{
		m_x.reserve(nPoints);
		m_indices.reserve(nFaces * 3);
		m_normals.reserve(nFaces);
		m_vertexNormals.reserve(nPoints);
	}

	void xTriangleMesh::release()
	{
		m_indices.clear();
		m_x.clear();
		m_normals.clear();
		m_vertexNormals.clear();
	}

	void xTriangleMesh::addFace(const unsigned int * const indices)
	{
		for (unsigned int i = 0u; i < 3; i++)
			m_indices.push_back(indices[i]);
	}

	void xTriangleMesh::addFace(const int * const indices)
	{
		for (unsigned int i = 0u; i < 3; i++)
			m_indices.push_back((unsigned int)indices[i]);
	}

	void xTriangleMesh::addVertex(const Vector3r &vertex)
	{
		m_x.push_back(vertex);
	}

	void SPH::xTriangleMesh::updateNormals()
	{
		m_normals.resize(numFaces());

		#pragma omp parallel default(shared)
		{
			#pragma omp for schedule(static)  
			for (int i = 0; i < (int)numFaces(); i++)
			{
				// Get first three points of face
				const Vector3r &a = m_x[m_indices[3 * i]];
				const Vector3r &b = m_x[m_indices[3 * i + 1]];
				const Vector3r &c = m_x[m_indices[3 * i + 2]];

				// Create normal
				Vector3r v1 = b - a;
				Vector3r v2 = c - a;

				m_normals[i] = v1.cross(v2);
				m_normals[i].normalize();
			}
		}
	}

	void SPH::xTriangleMesh::updateVertexNormals()
	{
		m_vertexNormals.resize(numVertices());


		for (unsigned int i = 0; i < numVertices(); i++)
		{
			m_vertexNormals[i].setZero();
		}

		for (unsigned int i = 0u; i < numFaces(); i++)
		{
			const Vector3r &n = m_normals[i];
			m_vertexNormals[m_indices[3 * i]] += n;
			m_vertexNormals[m_indices[3 * i + 1]] += n;
			m_vertexNormals[m_indices[3 * i + 2]] += n;
		}

		for (unsigned int i = 0; i < numVertices(); i++)
		{
			m_vertexNormals[i].normalize();
		}
	}
}