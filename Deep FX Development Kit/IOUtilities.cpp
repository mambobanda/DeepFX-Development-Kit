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


#include "IOUtilities.h"
#include <assimp/postprocess.h> 
#include <glm/vec2.hpp>

aiScene::aiScene() {
	mFlags = 0;
	mRootNode = NULL;
	mNumMeshes = 0;
	mMeshes = NULL;
	mNumMaterials = 0;
	mMaterials = NULL;
	mNumAnimations = 0;
	mAnimations = NULL;
	mNumTextures = 0;
	mTextures = NULL;
	mNumLights = 0;
	mLights = NULL;
	mNumCameras = 0;
	mCameras = NULL;
	mPrivate = NULL;
}


namespace IOUtilities {
	
	void meshDataToObj(std::string path, Dfx3dMeshData *mesh) {

		if (mesh->getVertexCount() > 0) {
			
			aiScene scene;
			Assimp::Exporter aiExporter;

			scene.mRootNode = new aiNode();

			scene.mMeshes = new aiMesh*[1];
			scene.mMeshes[0] = nullptr;
			scene.mNumMeshes = 1;
			scene.mMeshes[0] = new aiMesh();

			scene.mRootNode->mMeshes = new unsigned int[1];
			scene.mRootNode->mMeshes[0] = 0;
			//scene.mRootNode->mNumMeshes = 1;

			auto pMesh = scene.mMeshes[0];

			pMesh->mVertices = new aiVector3D[mesh->getVertexCount()];
			pMesh->mNormals = new aiVector3D[mesh->getVertexCount()];
			pMesh->mTextureCoords[0] = new aiVector3D[mesh->getVertexCount()];
			pMesh->mNumVertices = mesh->getVertexCount();
			pMesh->mNumUVComponents[0] = 2;
			pMesh->mNumUVComponents[1] = 0;
			pMesh->mNumUVComponents[2] = 0;
			pMesh->mNumUVComponents[3] = 0;
			pMesh->mNumUVComponents[4] = 0;
			pMesh->mNumUVComponents[5] = 0;
			pMesh->mNumUVComponents[6] = 0;
			pMesh->mNumUVComponents[7] = 0;


			for (size_t i = 0; i < mesh->getVertexCount(); i++)
			{
				pMesh->mVertices[i] = aiVector3D(mesh->getVertex(i).x, mesh->getVertex(i).y, mesh->getVertex(i).z);
				pMesh->mNormals[i] = aiVector3D(mesh->getNormal(i).x, mesh->getNormal(i).y, mesh->getNormal(i).z);
				pMesh->mTextureCoords[0][i] = aiVector3D(mesh->getUV(i).x, mesh->getUV(i).y, 0.0f);
			}
		
			pMesh->mFaces = new aiFace[mesh->getIndexCount() / 3];
			pMesh->mNumFaces = mesh->getIndexCount() / 3;
			
			scene.mRootNode->mNumMeshes = 1;

			scene.mMaterials = new aiMaterial*[1];
			scene.mMaterials[0] = nullptr;
			scene.mNumMaterials = 1;
			scene.mMaterials[0] = new aiMaterial();

			int counter = 0;
			for (size_t i = 0; i < mesh->getIndexCount() / 3; i++) {

				aiFace &face1 = pMesh->mFaces[i];

				face1.mIndices = new unsigned int[3];
				face1.mNumIndices = 3;

				face1.mIndices[0] = mesh->getIndex(counter);
				face1.mIndices[1] = mesh->getIndex(counter + 1);
				face1.mIndices[2] = mesh->getIndex(counter + 2);

				counter += 3;
			}
			
			aiReturn rc = aiExporter.Export(&scene, "obj", path, aiProcess_GenNormals);

			delete[] pMesh->mFaces;
			delete[] pMesh->mVertices;
			delete[] scene.mRootNode->mMeshes;

			delete scene.mMaterials[0];
			scene.mMaterials[0] = NULL;
			delete[] scene.mMaterials;
			
			delete[] scene.mMeshes;

			pMesh->mFaces = NULL;
			pMesh->mVertices = NULL;
			pMesh->mNormals = NULL;
			scene.mRootNode->mMeshes = NULL;
			scene.mMeshes = NULL;
			pMesh->mVertices = NULL;
			scene.mMaterials = NULL;
		}
	}
}