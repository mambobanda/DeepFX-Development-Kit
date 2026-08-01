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

#include <DfxGlobal.h>
#include <DfxScene.h>
#include <DfxObjectItem.h>
#include <vector>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h> 

struct DfxColladaSceneImporterSettings{
	unsigned int flags;
};

class DfxColladaSceneImporter {
private:
	void printBasicSceneInfo(const aiScene *);
public:
	DfxColladaSceneImporter();

	void importFile(std::string  str, DfxColladaSceneImporterSettings settings);

	void loadObject(const aiMesh *m, Dfx3dMeshData &mesh);

	void walkSceneGraph(const aiScene *scene, const aiNode *, std::vector<aiMatrix4x4> );
};
