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

#include <list>
#include <functional>
#include <string>
#include <queue>
#include <fstream>
#include <iostream>
#include <functional>
#include "DfxEntityCommon.h"
#include "DfxEntityDataInstance.h"


// Using Declarations
using DfxEntityChildList = std::list<DfxEntity*>;
using DfxEntityTraversalDoFunc = std::function< void(DfxEntity*) >;
using DfxEntityTraversalRunDoFunc = std::function< bool(DfxEntity*) >;
using DfxEntityTraversalSkipFunc = std::function< bool(DfxEntity*) >;

using DfxEntityID = size_t;
using DfxPropertyID = size_t;
using FreeDataCallback = void(*)(size_t);

class DfxEntity {
protected:
	static size_t ENTITY_INSTANCE_ID_GEN;
protected:
	DfxGlobalEntityName ORIGINAL_NAME;
	DfxEntityID globalID;
	DfxEntityID instanceID;
	FreeDataCallback freeCallback;
	DfxEntityChildList children;
	std::string logicalName;

	//Flags
	bool hidden;

	// Delegation
	std::vector<DfxEntity*> delegatees;
	std::vector<DfxEntity*> delegateTips;
	DfxEntity *delegator;
	
protected:
	void _createDataInstance();
	void _init();
	
public:
	DfxEntity();
	DfxEntity(DfxGlobalEntityName uniquename);
	virtual ~DfxEntity();
	
	virtual void compute() = 0;
	virtual void onDataChanged(DfxEntityDataInstance *instance);
	virtual void importData(std::vector<std::pair<std::string, std::string>> data);
	virtual std::vector<std::pair<std::string, std::string>> exportData();
	virtual bool isExportable();
	virtual void reset();
	virtual void save(std::ofstream &);
	virtual void load(std::ifstream &);
public:
	static void bind(DfxEntity *intputDfxEntity, size_t inputPropertyID, DfxEntity *outputDfxEntity, size_t outputPropertyID);
	static void unbind(DfxEntity *entity, size_t propertyID);
public:

	std::string getLogicalName();

	const DfxGlobalEntityName UniqueName();
	void setLogicalName(std::string str);

	size_t getInstanceID();

	size_t getGlobalID();

	virtual void initialize();

	virtual void copy(DfxEntity &);

	void setFreeCallback(FreeDataCallback);

	DfxEntityDataInstance *getDataInstance();

	void *getProperty(size_t);

	std::string getPropertyName(size_t);

	size_t getPropertyType(size_t);

	void setPropertyState(size_t pid, DfxEntityPropertyState st);

	bool propertyHasBinding(size_t);

	void addChild(DfxEntity *);

	void removeChild(DfxEntity *);

	void removeChild(size_t instance_id);

	DfxEntity* queryChild(size_t instance_id);

	DfxEntityChildList getAllChildren();

	bool hasChildren();

	bool hasChild(DfxEntity *);

	bool hasParent();

	bool parentsAreClean();

	void bfsQuery(DfxEntity *root);

	void makeDirty();

	bool branchIsClean();

	void makeChildrenDirty();

	void unbindOutput(size_t inid);

	void unbindEverything();

	void pingParents(bool pingSelf = true);

	void makePropertyBindingDirty(size_t inid);

	bool isPropertyBindingDirty(size_t);

	void cleanPropertyBinding(size_t);

	void cleanAllProperties();

	void dirtyAllProperties();

	static size_t hash(DfxGlobalEntityName uniquename);

	bool hasDataStoreProperty(std::string);

	size_t dataStorePropertyID(std::string);

	void hide();

	void unhide();

	bool isHidden();

	bool isDelegate();

	void clearDelegator();

	DfxEntity *getDelegator();

	void setDelegator(DfxEntity *);

	std::vector<DfxEntity*> getAllDelegatees();

	DfxEntity *getDelegatee(size_t uid);

	void addDelegatee(DfxEntity *);

	void removeDelegatee(size_t uid);

	void clearDelegatees();

	bool delegateesAreClean();

	std::vector<DfxEntity*> getDelegateTips();
};
