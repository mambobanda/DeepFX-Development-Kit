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

#include "DfxPluginManager.h"


DfxPluginManager *DfxPluginManager::instance =  nullptr;
std::unordered_map < std::string, void *(*)()> DfxPluginManager::creator_map;

DfxPluginManager::DfxPluginManager() {
	//vdbSkinnerPluginActive = false;
}

DfxPluginManager::~DfxPluginManager() {

}

DfxPluginManager *DfxPluginManager::getInstance() {

	if (instance == nullptr) {
		instance = new DfxPluginManager();
	}

	return instance;
}

#ifdef WIN32

void *DfxPluginManager::findDfxPluginInitFunction(std::string dllpath) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	HINSTANCE hLib = LoadLibrary(converter.from_bytes(dllpath).c_str());

	if (hLib != NULL) {

		DfxInitFunc hc = (DfxInitFunc)GetProcAddress(hLib, "init_dfx_plugin");

		if (hc != NULL) {
			return hc;
		}
		else {
			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

//void DfxPluginManager::findLoadVDBSkinnerPlugin(std::string dllpath) {
//
//	std::filesystem::path p(dllpath);
//	
//	if (std::filesystem::exists(p)){
//		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
//
//			HINSTANCE hLib = LoadLibrary(converter.from_bytes(dllpath).c_str());
//
//			if (hLib != NULL) {
//
//				vdbParticleSkinnerFunc = (DfxVDBSkinParticlesFunc)GetProcAddress(hLib, "skinParticles");
//
//					if (vdbParticleSkinnerFunc != NULL) {
//						vdbSkinnerPluginActive = true;
//
//							skinnerMeshGlobal = new DfxVDBOutputMeshFunctions;
//						skinnerMeshGlobal->new_mesh = vdb::new_mesh;
//						skinnerMeshGlobal->init_vertex_array = vdb::init_vertex_array;
//						skinnerMeshGlobal->init_index_array = vdb::init_index_array;
//						skinnerMeshGlobal->msg = vdb::msg;
//
//						DfxGlobal::console::println("VDB Particle Skinner Plugin Loaded Successfully.");
//						return;
//					}
//					else {
//						DfxGlobal::console::println("VDB Particle Skinner Plugin Load Failed: No Interface found.");
//						return;
//					}
//			}
//			else {
//				DfxGlobal::console::println("VDB Particle Skinner Plugin Load Failed: Invalid Plugin.");
//				return;
//			}
//	}
//	else {
//		DfxGlobal::console::println("VDB Particle Skinner Plugin Load Failed: Could not find Plugin.");
//	}
//}

void DfxPluginManager::unloadVDBSkinnerPlugin() {

}

void DfxPluginManager::loadPlugins(std::string dir, DfxSDKGlobal *global) {
	
	if (filesystem::is_directory(dir)) {

		for (auto &fs : filesystem::directory_iterator(dir)) {
			if (fs.path().extension().string().compare(".dll") == 0) {
				DfxInitFunc func = (DfxInitFunc)findDfxPluginInitFunction(fs.path().string());

				if (func != nullptr) {
					func(global);
				}
				else {
					DfxGlobal::console::println(std::string("No Plugins found in: ") + fs.path().string());
				}
			}
		}
	}
	else {
		DfxGlobal::console::println(std::string("Unknown Plugin Directory: ") + dir);
	}
}

void *DfxPluginManager::creator(DfxGlobalEntityName uniquename) {

	auto it = creator_map.find(uniquename);

	if (it != creator_map.end()) {
		return it->second();
	}
	else {
		return nullptr;
	}
}

void DfxPluginManager::addCreator(DfxGlobalEntityName uniquename, void *(*func)()) {
	creator_map.insert(std::pair < std::string, void *(*)() > (uniquename, func));
}

//bool DfxPluginManager::isVdbSkinnerPluginActive() {
//	return vdbSkinnerPluginActive;
//}
//
//DfxVDBSkinParticlesFunc DfxPluginManager::getVDBSkinnerParticlesFunc() {
//	return vdbParticleSkinnerFunc;
//}
//
//DfxVDBOutputMeshFunctions *DfxPluginManager::getVDBMeshGlobal() {
//	return skinnerMeshGlobal;
//}
//
//namespace vdb {
//	DfxVDBOutputMesh *new_mesh() {
//		return new DfxVDBOutputMesh;
//	}
//
//	void init_vertex_array(DfxVDBOutputMesh *mesh, unsigned int size) {
//		mesh->verts = new DfxVDBFloat3[size];
//		mesh->normals = new DfxVDBFloat3[size];
//	}
//
//	void init_index_array(DfxVDBOutputMesh *mesh, unsigned int size) {
//		mesh->indices = new unsigned int[size];
//	}
//
//	void msg(const char *ms) {
//		DfxGlobal::console::println(ms);
//	}
//}

#endif