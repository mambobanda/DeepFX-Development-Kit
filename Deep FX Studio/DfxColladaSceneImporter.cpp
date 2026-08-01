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

#include <EulerAngles.h>
#include "DfxColladaSceneImporter.h"

DfxColladaSceneImporter::DfxColladaSceneImporter() {

}

void DfxColladaSceneImporter::printBasicSceneInfo(const aiScene *scene){

	DfxGlobal::console::println(std::string("-Importing Collada Scene ") + std::to_string(scene->mNumAnimations));

	DfxGlobal::console::println(std::string("\t-Number of Animations: ") + std::to_string(scene->mNumAnimations));
	
	DfxGlobal::console::println(std::string("\t-Number of Cameras: ") + std::to_string(scene->mNumCameras));
		
	DfxGlobal::console::println(std::string("\t-Number of Lights: ") + std::to_string(scene->mNumLights ));
	
	DfxGlobal::console::println(std::string("\t-Number of Materials: ") + std::to_string(scene->mNumMaterials));
	
	DfxGlobal::console::println(std::string("\t-Number of Meshes: ") + std::to_string(scene->mNumMeshes));
	
	DfxGlobal::console::println(std::string("\t-Number of Textures: ") + std::to_string(scene->mNumTextures));
		
}

void DfxColladaSceneImporter::walkSceneGraph(const aiScene *scene, const aiNode *n, std::vector<aiMatrix4x4> mat_array) {

	aiVector3D pos;
	aiVector3D scl;
	aiQuaternion rot;
	glm::vec3 euler;;
	aiMatrix4x4 mat;

	mat_array.push_back(n->mTransformation);

	if (n->mNumMeshes > 0) {
		
		for (int i = 0; i < mat_array.size(); i++) {
			mat *= mat_array[i];
		}
		
		EulerAngles angles = { 0,0,0,EulOrdXYZr };
		HMatrix matr;

		matr[0][0] = mat.a1; matr[0][1] = mat.a2; matr[0][2] = mat.a3; matr[0][3] = mat.a4;
		matr[1][0] = mat.b1; matr[1][1] = mat.b2; matr[1][2] = mat.b3; matr[1][3] = mat.b4;
		matr[2][0] = mat.c1; matr[2][1] = mat.c2; matr[2][2] = mat.c3; matr[2][3] = mat.c4;
		matr[3][0] = mat.d1; matr[3][1] = mat.d2; matr[3][2] = mat.d3; matr[3][3] = mat.d4;
		
		angles = Eul_FromHMatrix(matr, EulOrdXYZr);

		mat.Decompose(scl, rot, pos);
		
		euler.x = angles.x;
		euler.y = angles.y;
		euler.z = angles.z;
	}

	for (int j = 0; j < n->mNumMeshes; j++) {

		auto amesh = scene->mMeshes[n->mMeshes[j]];
		
		if (amesh->mNumVertices >= 3) {
			DfxObjectItem *oitem = new DfxObjectItem(DfxObjectItem_Identifier);

			loadObject(amesh, *oitem->getMeshData());

			oitem->setLogicalName(amesh->mName.C_Str());
			oitem->initOpenGlBuffer();
			oitem->setIsReady(true);
			oitem->setFilerReferenced(false);

			oitem->setPosition(glm::vec3(pos.x, pos.y, pos.z));
			oitem->setScale(glm::vec3(scl.x, scl.y, scl.z));
			oitem->setRotation(glm::vec3(glm::degrees(euler.x), 
							   glm::degrees(euler.y),
							   glm::degrees(euler.z)));

			oitem->calculateBounds();

			DfxScene::getInstance()->addItem(oitem);
		}

	}

	if (n->mNumChildren > 0) {

		for (int i = 0; i < n->mNumChildren; i++) {	
			walkSceneGraph(scene, n->mChildren[i], mat_array);
		}
	}
}

void DfxColladaSceneImporter::importFile( std::string path, DfxColladaSceneImporterSettings settings ) {

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, settings.flags);

	if (!scene) {
		DfxGlobal::console::println("ERROR: Failed to load Collada scene.\n\n");
	}
	else {

		printBasicSceneInfo(scene);

		std::vector<aiMatrix4x4> mat;

		walkSceneGraph(scene, scene->mRootNode, mat);
	}
}

void DfxColladaSceneImporter::loadObject(const aiMesh *m, Dfx3dMeshData &mesh) {

	unsigned int tverts = m->mNumVertices;
	unsigned int tfaces = m->mNumFaces;

	mesh.resizeVertexBuffer(tverts);
	mesh.resizeIndexBuffer(tfaces * 3);

	int vindex = 0;
	int iindex = 0;

	for (int j = 0; j < m->mNumVertices; j++) {

		mesh.setVertex(j, glm::vec3(m->mVertices[j].x, m->mVertices[j].y, m->mVertices[j].z));

		if(m->mNormals != NULL)
			mesh.setNormal(j, glm::vec3(m->mNormals[j].x, m->mNormals[j].y, m->mNormals[j].z));
		else
			mesh.setNormal(j, glm::vec3(0.0f, 0.0f, 0.0f));

		if (m->mTextureCoords != NULL && m->mNumUVComponents[0] > 0) {
			mesh.setUV(j, glm::vec2(m->mTextureCoords[0][j].x, m->mTextureCoords[0][j].y));
		}
		else {
			mesh.setUV(j, glm::vec2(.0f, .0f));
		}
		
		mesh.setColor(j, glm::vec3(0.69f, 0.69f, 0.69f));
	}

	for (int j = 0; j < m->mNumFaces; j++) {

		if (m->mFaces[j].mIndices[0] > (m->mNumFaces * 3) ||
			m->mFaces[j].mIndices[1] > (m->mNumFaces * 3) ||
			m->mFaces[j].mIndices[2] > (m->mNumFaces * 3)) {
			continue;
		}
	
		mesh.setIndex(iindex, m->mFaces[j].mIndices[0]);
		mesh.setIndex(iindex + 1, m->mFaces[j].mIndices[1]);
		mesh.setIndex(iindex + 2, m->mFaces[j].mIndices[2]);

		iindex += 3;
	}
}