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

  
#include <DsMesh.h>
#include <DsMeshBuilder.h>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include "DfxSurfaceDictionary.h"
#include "DfxCommon.h"

class DfxObjectLoader {
private:
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<glm::vec3> verts;

	std::string warn;
	std::string err;
	
	size_t obj_count;

public:
	
	ds::Mesh *loadObject(std::string path, std::string objPath, void(*callback)(int current, int total) = nullptr);
	ds::Mesh *loadObject2(std::string path, void(*callback)(int current, int total) = nullptr);

	bool load(std::string path);
	void readObjs(std::vector<ds::Mesh*> mshLst, void(*callback)(int current,int total) = nullptr);
	size_t objCount();

};