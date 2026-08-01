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
#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <atomic>

//#define DFX_MESH_ATTRR_COUNT 3
//#define DFX_MESH_VEC_LENGTH  3

namespace DfxEntityPropertyTypes{
	typedef struct st_float3 {
		float x;
		float y;
		float z;

		st_float3() {
			x = 0.f; y = 0.f; z = 0.f;
		}

	}Float3;
}

class Dfx3dMeshData {
private:
	static std::atomic<size_t> ID_GEN;
	size_t id;
private:
	glm::vec3 *verticies;
	glm::vec3 *normals;
	glm::vec3 *colors;
	glm::vec2 *uvs;
	size_t *indices;
	
	size_t vertexCount;
	size_t indexCount;

	unsigned int vertexBufferBytes;
	unsigned int indexBufferBytes;
public:

	Dfx3dMeshData();
	~Dfx3dMeshData();
	
	void clearVertexBuffer();
	void clearIndexBuffer();
	void resizeVertexBuffer(size_t);
	void resizeIndexBuffer(size_t);
	
	glm::vec3 getVertex(size_t index);
	DfxEntityPropertyTypes::Float3 _getVertex(size_t index);
	void setVertex(size_t index, glm::vec3);
	void setVertex(size_t index, float x, float y, float z);
	void _setVertex(size_t index, float x, float y, float z);

	glm::vec2 getUV(size_t index);
	DfxEntityPropertyTypes::Float3 _getUV(size_t index);
	void setUV(size_t index, glm::vec2);
	void setUV(size_t index, float x, float y, float z);
	void _setUV(size_t index, float x, float y, float z);

	glm::vec3 getNormal(size_t index);
	DfxEntityPropertyTypes::Float3 _getNormal(size_t index);
	void setNormal(size_t index, glm::vec3);
	void setNormal(size_t index, float x, float y, float z);
	void _setNormal(size_t index, float x, float y, float z);

	glm::vec3 getColor(size_t index);
	DfxEntityPropertyTypes::Float3 _getColor(size_t index);
	void setColor(size_t index, glm::vec3);
	void setColor(size_t index, float r, float g, float b);
	void _setColor(size_t index, float r, float g, float b);

	size_t getIndex(size_t inx);
	void   setIndex(size_t, size_t  val);

	size_t getVertexCount();
	size_t getIndexCount();

	glm::vec3 *getVertexBuffer();
	size_t *getIndexBuffer();

	unsigned int getVertexBufferBytes();
	unsigned int getIndexBufferBytes();

	void calculateVertexNormals();

	void copy(Dfx3dMeshData &);

	size_t getID() const;

	void transformData(glm::mat4 &);
};