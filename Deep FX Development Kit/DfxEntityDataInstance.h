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

#include <unordered_map>
#include <vector>
#include <thread>
#include "DfxEntityCommon.h"
#include "DfxEntityDataListener.h"
#include "DfxEntityProperty.h"

class DfxEntityDataInstance{
private:
	std::vector<DfxEntityDataListener> eventListeners;
	std::unordered_map<size_t, DfxEntityProperty*> instanceData;
	DfxEntityProperty* findProperty(size_t);
private:
	std::mutex  eventListenerLock;
private:
	DfxEntity *entity;
	bool clean;
public:

	DfxEntityDataInstance();
	DfxEntityDataInstance(DfxEntity *);
	virtual ~DfxEntityDataInstance();

	void insertProperty(size_t, DfxEntityProperty*);

	DfxEntityProperty *getPropertyInstance(size_t id);

	void *getProperty(size_t);

	std::string getPropertyName(size_t);
	
	size_t getPropertyType(size_t);

	std::vector<size_t> getPropertyList();

	void addEventListener(DfxEntityDataListener);

	void removeEventListener(DfxEntityDataListener);

	void beginNotify();

	void endNotify();

	bool isClean();

	void makeClean();

	void makeDirty();

	DfxEntity *getDfxEntity();
};