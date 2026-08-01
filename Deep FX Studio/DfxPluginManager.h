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
#include <DfxSDKEntry.h>
#include <DfxGlobal.h>
#include <experimental\filesystem>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
#include <unordered_map>
//#include <DfxVDBSkinner.h>
//#include <DfxVDBOutputMesh.h>

#ifdef WIN32
#include <Windows.h>
#endif

namespace filesystem = std::experimental::filesystem;
//using DfxVDBSkinParticlesFunc = DfxVDBOutputMesh *(*)(DfxVDBFloat3 *, unsigned int pointCount, float gridRadius, float particleRadius, float halfWidth, VDBFilter *filters, unsigned int filterCount, DfxVDBOutputMeshFunctions *meshFunc);

class DfxPluginManager {
private:
	static DfxPluginManager *instance;

	static std::unordered_map < std::string, void *(*)() > creator_map;

	DfxPluginManager();
private:
	/*bool vdbSkinnerPluginActive;
	DfxVDBOutputMeshFunctions *skinnerMeshGlobal;
	DfxVDBSkinParticlesFunc vdbParticleSkinnerFunc;*/
public:
	~DfxPluginManager();

	static DfxPluginManager *getInstance();
	void loadPlugins(std::string , DfxSDKGlobal *);
	void addCreator(DfxGlobalEntityName uniquename, void *(*)());
	//create_func findCreator(DfxGlobalEntityName uniquename);

#ifdef WIN32
	void *findDfxPluginInitFunction(std::string dllpath);
	void findLoadVDBSkinnerPlugin(std::string dllpath);
	void unloadVDBSkinnerPlugin();
#endif

	/*DfxVDBSkinParticlesFunc getVDBSkinnerParticlesFunc();
	DfxVDBOutputMeshFunctions *getVDBMeshGlobal();
	bool isVdbSkinnerPluginActive();*/

	static void *creator(DfxGlobalEntityName uniquename);
};


using DfxInitFunc = int(*)(DfxSDKGlobal *);

namespace vdb {

	/*DfxVDBOutputMesh *new_mesh();

	void init_vertex_array(DfxVDBOutputMesh *mesh, unsigned int size);

	void init_index_array(DfxVDBOutputMesh *mesh, unsigned int size);

	void msg(const char *ms);*/
}