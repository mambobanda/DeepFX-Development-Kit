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

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glm/vec2.hpp>
#include "DfxGlobal.h"
#include "DfxObjLoaderPlugin.h"


DfxObjLoaderPlugin::DfxObjLoaderPlugin(DfxGlobalEntityName uniquename): DfxObjectLoaderService(uniquename) {

}

void DfxObjLoaderPlugin::loadObject(Dfx3dMeshData &mesh) {

	std::string pFile = getFilename();

	if (pFile.size() <= 0) {
		setStatus(FILE_NOT_FOUND);
		DfxGlobal::console::println("File Not Found.");
		return;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile, aiProcess_Triangulate);

	if (!scene) {
		setStatus(UNABLE_TO_READ_FILE);
		DfxGlobal::console::println("Unable To Read File.");
		return;
	}

	aiMesh *m = scene->mMeshes[0];

	unsigned int tverts = m->mNumVertices;
	unsigned int tfaces = m->mNumFaces;

	/*std::stringstream ss;

	for (int j = 0; j < m->mNumVertices; j++) {

		ss << "[ " << m->mVertices[j].x << ", " << m->mVertices[j].y << ", " <<	m->mVertices[j].z << " ]\n";
	}

	ss << "Number Of Verts: " << tverts << std::endl;
	ss << "Number Of faces: " << tfaces;

	DfxGlobal::console::println(ss.str());*/

    mesh.resizeVertexBuffer(tverts);
	mesh.resizeIndexBuffer(tfaces * 3);

	int vindex = 0;
	int iindex = 0;

	for (int j = 0; j < m->mNumVertices; j++) {
		
		mesh.setVertex(j, glm::vec3(m->mVertices[j].x, m->mVertices[j].y, m->mVertices[j].z));
		mesh.setNormal(j, glm::vec3(m->mNormals[j].x, m->mNormals[j].y, m->mNormals[j].z));
		mesh.setColor(j, glm::vec3(0.69f, 0.69f, 0.69f));

		if (m->mTextureCoords != NULL && m->mNumUVComponents[0] > 0) {
			mesh.setUV(j, glm::vec2(m->mTextureCoords[0][j].x, m->mTextureCoords[0][j].y));
		}
		else {
			mesh.setUV(j, glm::vec2(.0f, .0f));
		}
	}

	for (int j = 0; j < m->mNumFaces; j++) {
		mesh.setIndex(iindex, m->mFaces[j].mIndices[0]);
		mesh.setIndex(iindex + 1, m->mFaces[j].mIndices[1]);
		mesh.setIndex(iindex + 2, m->mFaces[j].mIndices[2]);

		iindex += 3;
	}

	setStatus(SUCCESS);
}

void DfxObjLoaderPlugin::initialize() {
	DfxObjectLoaderService::initialize();
}

void *DfxObjLoaderPlugin::createService(DfxGlobalEntityName uniquename) {

	return new DfxObjLoaderPlugin(uniquename);
}