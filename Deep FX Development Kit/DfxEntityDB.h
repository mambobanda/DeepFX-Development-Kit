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

#include <functional>
#include <unordered_map>
#include "DfxEntityCommon.h"
#include "DfxEntityDefinition.h"
#include "DfxEntity.h"

class DfxEntityDB {
private:
	static size_t activeDefinitionID;
	static size_t activeDefinitionPropertyIDCounter;
	static DfxEntityDefintion *activeDefinition;
private:
	using CreatorFuncPtr = void*(*)(size_t);

	static std::unordered_map<size_t, DfxEntityDefintion*> definitionMap;
	static std::unordered_map<size_t, DfxEntityDataInstance*> instances;
	static std::unordered_map<size_t, void*(*)(DfxGlobalEntityName )> creatorMap;
public:
	static void beginDefintion(size_t ent_id);
	static void endDefintion();

	static void createProperty(size_t propID, std::string name, DfxEntityPropertyType *type);

	static void newDfxEntityDataInstance(size_t, size_t);

	static void newDfxEntityDataInstance(size_t, size_t, DfxEntity *);

	static void deleteDataInstance(size_t instance_id);

	static DfxEntityDataInstance* getDataInstance(size_t);

	static void registerDfxEntity(DfxGlobalEntityName uniquename, void(*initFunc)(), void*(*)(DfxGlobalEntityName) = nullptr);

	static void insertDataInstance(size_t instanceID, DfxEntityDataInstance*);

	//static void registerDfxEntity(const char *uniquename, void(*initFunc)(), void*(*)(const char*) = nullptr);

	static DfxEntity *newEntityInstance(DfxGlobalEntityName uniquename);
};

