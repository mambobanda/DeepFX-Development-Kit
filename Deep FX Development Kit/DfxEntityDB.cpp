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

#include "DfxEntityDB.h"

size_t DfxEntityDB::activeDefinitionPropertyIDCounter = 0;
DfxEntityDefintion *DfxEntityDB::activeDefinition = nullptr;
size_t DfxEntityDB::activeDefinitionID = 0;
std::unordered_map<size_t, DfxEntityDefintion*> DfxEntityDB::definitionMap;
std::unordered_map<size_t, DfxEntityDataInstance*> DfxEntityDB::instances;
std::unordered_map<size_t, void*(*)(DfxGlobalEntityName)> DfxEntityDB::creatorMap;

void DfxEntityDB::beginDefintion(size_t ent_id) {
	activeDefinitionID = ent_id;
	activeDefinition = new DfxEntityDefintion();
}

void DfxEntityDB::endDefintion() {
	if (activeDefinition != nullptr) {
		std::pair<size_t, DfxEntityDefintion*> p(activeDefinitionID, activeDefinition);
		definitionMap.insert(p);

		activeDefinition = nullptr;
		activeDefinitionID = 0;
		DfxEntityDB::activeDefinitionPropertyIDCounter = 0;
	}
}

void DfxEntityDB::createProperty(size_t propID, std::string name, DfxEntityPropertyType *type) {
	DfxEntityPropertySpec spec;
	spec.name = name;
	spec.typeInstance = type;
	spec.property_id = propID;

	activeDefinition->insertSpecification(spec);
}

void DfxEntityDB::newDfxEntityDataInstance(size_t entity_id, size_t instance_id) {
	std::unordered_map<size_t, DfxEntityDefintion*>::iterator it = definitionMap.find(entity_id);

	if (it != definitionMap.end()) {
		DfxEntityDataInstance* newInstance = it->second->createInstance();

		std::pair<size_t, DfxEntityDataInstance*> p(instance_id, newInstance);
		instances.insert(p);

	}
	else {

	}
}

void DfxEntityDB::newDfxEntityDataInstance(size_t entity_id, size_t instance_id, DfxEntity *ent) {
	std::unordered_map<size_t, DfxEntityDefintion*>::iterator it = definitionMap.find(entity_id);

	if (it != definitionMap.end()) {
		DfxEntityDataInstance* newInstance = it->second->createInstance(ent);

		std::pair<size_t, DfxEntityDataInstance*> p(instance_id, newInstance);
		instances.insert(p);

	}
	else {

	}
}

void DfxEntityDB::insertDataInstance(size_t instance_id, DfxEntityDataInstance* newInstance) {
	std::pair<size_t, DfxEntityDataInstance*> p(instance_id, newInstance);
	instances.insert(p);
}

DfxEntityDataInstance* DfxEntityDB::getDataInstance(size_t instance_id) {
	std::unordered_map<size_t, DfxEntityDataInstance*>::iterator it = instances.find(instance_id);

	if (it != instances.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

void DfxEntityDB::registerDfxEntity(const DfxGlobalEntityName uniquename, void(*initFunc)(), void*(*creatorFunc)(DfxGlobalEntityName)) {

	size_t unique_id = DfxEntity::hash(uniquename);
	DfxEntityDB::beginDefintion(unique_id);

	initFunc();

	DfxEntityDB::endDefintion();

	creatorMap.emplace(unique_id, creatorFunc);
}

void DfxEntityDB::deleteDataInstance(size_t instance_id) {
	std::unordered_map<size_t, DfxEntityDataInstance*>::iterator it = instances.find(instance_id);

	if (it != instances.end()) {
		delete it->second;
	}

	instances.erase(instance_id);
}

DfxEntity *DfxEntityDB::newEntityInstance(DfxGlobalEntityName uniquename) {

	auto it = creatorMap.find(DfxEntity::hash(uniquename));

	if (it != creatorMap.end()) {
		return (DfxEntity*)it->second(uniquename);
	}

	return nullptr;
}