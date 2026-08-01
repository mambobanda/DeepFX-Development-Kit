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

#include <string>

class DfxCacheSimplexInfo {
public:
	// Vertex Buffer Properties
	bool updateVertexBuffer;
	std::string vertexBufferReference;
	int vertexBufferVersion;

	// Vertex Normal Buffer Properties
	bool updateVertexNormalBuffer;
	std::string vertexNormalBufferReference;
	int vertexNormalBufferVersion;

	// uv Buffer Properties
	bool updateUvBuffer;
	std::string uvBufferReference;
	int uvBufferVersion;

	// materialID Buffer Properties
	bool updateMaterialIDBuffer;
	std::string materialIDBufferReference;
	int materialIDBufferVersion;

	// index Buffer Properties
	bool updateIndexBuffer;
	std::string indexBufferReference;
	int indexBufferVersion;

	// Topology Properties
	int topologyVersion;
};