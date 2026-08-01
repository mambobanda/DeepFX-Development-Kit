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

#include <DsTools.h>
#include <OBJ_Loader.h>
#include "DfxObjectLoader.h"
#include "DfxUtil.h"
#include "DfxUtility.h"

bool DfxObjectLoader::load(std::string path) {

	std::string pFile = path;

	if (pFile.size() <= 0) {
		DfxGlobal::console::println("Invaled file path, failed to load Object");
		return false;
	}

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pFile.c_str(), 0, false);

	if (!warn.empty()) {
		DfxGlobal::console::println(warn);
	}

	if (!err.empty()) {
		DfxGlobal::console::println(err);
	}

	if (!ret) {
		DfxGlobal::console::println("Failed to load OBJ.");
		return false;
	}

	if (shapes.size() <= 0) {
		DfxGlobal::console::println("Failed to load OBJ: No mesh data found.");
		return false;
	}

	obj_count = (int)shapes.size();

	//printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	//printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	//printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	//printf("# of materials = %d\n", (int)materials.size());
	//printf("# of shapes    = %d\n", (int)shapes.size());
	
	for (int i = 0; i < attrib.vertices.size() / 3; i++) {
		size_t current = 3 * i;
		verts.push_back(glm::vec3(attrib.vertices[current], attrib.vertices[current + 1], attrib.vertices[current + 2]));
	}

	return true;
}

size_t DfxObjectLoader::objCount() {
	return obj_count;
}

void DfxObjectLoader::readObjs(std::vector<ds::Mesh*> mshLst, void(*callback)(int current, int total)) {

	for (size_t i = 0; i < obj_count; i++) {

		if (callback != nullptr) {
			callback(i, obj_count);
		}
		
		ds::MeshBuilder meshBuilder;
		meshBuilder.setMesh(mshLst[i]);

		meshBuilder.addVertices(verts);

		size_t index_count = 0;
		for (int j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
			std::vector<size_t> indices;

			for (int k = 0; k < shapes[i].mesh.num_face_vertices[j]; k++) {
				tinyobj::index_t ind_t = shapes[i].mesh.indices[index_count];
				indices.push_back(ind_t.vertex_index);
				index_count++;
			}

			meshBuilder.addFace(indices);
		}

		meshBuilder.getMesh()->reindexVertices();
		meshBuilder.getMesh()->reindexEdges();
		meshBuilder.getMesh()->reindexFaces();

	}
}

ds::Mesh *DfxObjectLoader::loadObject(std::string path, std::string objPath, void(*callback)(int current, int total)) {

	std::string pFile = path;
	std::unordered_map<std::string, size_t> surface_map;

	if (pFile.size() <= 0) {
		DfxGlobal::console::println("Invalid file path, failed to load Object");
		return nullptr;
	}

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, pFile.c_str(), 0, false);

	if (!warn.empty()) {
		// DfxGlobal::console::println(warn);
	}

	if (!err.empty()) {
		DfxGlobal::console::println(err);
	}

	if (!ret) {
		DfxGlobal::console::println("Failed to load OBJ.");
		return nullptr;
	}

	if (shapes.size() <= 0) {
		DfxGlobal::console::println("Failed to load OBJ: No mesh data found.");
		return nullptr;
	}

	/*
	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());
	printf("# of shape #0 indices = %d\n", (int)shapes[0].mesh.indices.size());
	*/

	DfxSurfaceDictionary::getInstance()->create(objPath);

	for (long long int inx = 0; inx < shapes.size(); inx++) {

		std::string name = shapes[inx].name;
		unsigned int surf_id = DfxSurfaceDictionary::getInstance()->maxID(objPath) + 1;
		DfxSurfaceDictionary::getInstance()->insert(objPath, surf_id, name);
		surface_map[name] = surf_id;

		for (int j = 0; j < shapes[inx].mesh.indices.size() / 3; j++) {
			tinyobj::index_t ind_t00 = shapes[inx].mesh.indices[3 * j + 0];
			tinyobj::index_t ind_t01 = shapes[inx].mesh.indices[3 * j + 1];
			tinyobj::index_t ind_t02 = shapes[inx].mesh.indices[3 * j + 2];

			// std::cout << "index: (" << ind_t00.vertex_index << ", " << ind_t01.vertex_index << ", " << ind_t02.vertex_index << ")\n";
		}
	}

	std::vector<glm::vec3> m_verts;
	for (int i = 0; i < attrib.vertices.size() / 3; i++) {
		size_t current = 3 * i;
		m_verts.push_back(glm::vec3(attrib.vertices[current], attrib.vertices[current + 1], attrib.vertices[current + 2]));
	}

	auto msh = dstools::newStandardMesh();
	ds::MeshBuilder meshBuilder;
	meshBuilder.setMesh(msh);

	meshBuilder.addVertices(m_verts);
	meshBuilder.getMesh()->reindexVertices();

	for (int i = 0; i < shapes.size(); i++) {

		size_t index_count = 0;
		for (int j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++) {
			std::vector<size_t> indices;
			std::vector<glm::vec2> uvs;

			for (int k = 0; k < shapes[i].mesh.num_face_vertices[j]; k++) {

				tinyobj::index_t ind_t = shapes[i].mesh.indices[index_count];

				int vindex = ind_t.vertex_index;
				float u = 0.f; 
				float v = 0.f;
				
				if (ind_t.texcoord_index > 0) {
					u = attrib.texcoords[2 * ind_t.texcoord_index];
					v = -attrib.texcoords[2 * ind_t.texcoord_index + 1];
				}
				
				uvs.push_back(glm::vec2(u, v));
				indices.push_back(vindex);
				
			

				index_count++;
			}

			auto face = meshBuilder.addFace(indices);
			face->setSurfID(surface_map[shapes[i].name]);

			for (int k = 0; k < face->verticesCount(); k++) {
				face->uvs()[k] = uvs[k];		
			}

		}
	}
	
	meshBuilder.getMesh()->reindexEdges();
	meshBuilder.getMesh()->reindexFaces();
	

	return msh;
}

ds::Mesh *DfxObjectLoader::loadObject2(std::string path, void(*callback)(int current, int total)) {

	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(path);

	// If so continue
	if (loadout) {

		objl::Mesh curMesh = Loader.LoadedMeshes[0];

		// Print Mesh Name
		// std::cout << "Mesh Name" << curMesh.MeshName << "\n";

		std::string name = curMesh.MeshName;
		unsigned int surf_id = 0;

		// Go through each vertex and print its number,
		// position, normal, and texture coordinate
		std::vector<glm::vec3> m_verts;
		std::vector<glm::vec3> m_norms;
		std::vector<glm::vec2> m_uvs;
		for (int j = 0; j < curMesh.Vertices.size(); j++) {

			m_verts.push_back(glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z));
			m_norms.push_back(glm::vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z));
			m_uvs.push_back(glm::vec2(curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y));

		}

		auto msh = dstools::newStandardMesh();
		ds::MeshBuilder meshBuilder;
		meshBuilder.setMesh(msh);

		meshBuilder.addVertices(m_verts);
		meshBuilder.getMesh()->reindexVertices();

		std::cout << "Verts: "<< msh->vertices().size() << std::endl;

		for (int j = 0; j < msh->vertices().size(); j++) {
			msh->vertices()[j]->setProperty<glm::vec2>("uvs", m_uvs[j]);
		}

		//  Go through every 3rd index and print the
		//	triangle that these indices represent
		for (int j = 0; j < curMesh.Indices.size() / 3; j++) {	

			auto offset = j * 3;

			std::vector<size_t> indices;

			indices.push_back(curMesh.Indices[offset]);
			indices.push_back(curMesh.Indices[offset + 1]);
			indices.push_back(curMesh.Indices[offset + 2]);

			auto face = meshBuilder.addFace(indices);
			face->setSurfID(surf_id);
		}

		meshBuilder.getMesh()->reindexEdges();
		meshBuilder.getMesh()->reindexFaces();

		return msh;
	}

	return nullptr;
}