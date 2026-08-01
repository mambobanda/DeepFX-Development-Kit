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

#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include "DfxEntityCommon.h"
#include "DfxEntityDataInstance.h"
#include "DfxEntity.h"
#include "DfxEntityExceptions.h"
#include "DfxEntityPropertyType.h"
#include "DfxEntityDataListener.h"

class DfxEntityProperty {
private:
	std::vector<DfxEntityDataListener> eventListeners;
	void notifyEventListeners();
private:
	size_t id;
	void *data;
	std::string name;
	size_t type;

	DfxEntityPropertyState state;

	DfxEntityProperty *parent;

	DfxEntityDataInstance *source;

	bool parentClean;
	bool freeOnDeath;
private:
	std::mutex  eventListenerLock;
public:

	DfxEntityProperty(size_t id, DfxEntityPropertyType *type, std::string name, DfxEntityDataInstance *source );
	virtual ~DfxEntityProperty();

	void* getData();
	void setData(void *);
	std::string getName();
	size_t getType();

	size_t getID();

	void addEventListener(DfxEntityDataListener);

	void removeEventListener(DfxEntityDataListener);

	void beginNotify();

	void endNotify();

	void bind(DfxEntityProperty *);

	void unbind();

	bool hasBinding();

	bool isBindingClean();

	void setState(DfxEntityPropertyState);

	DfxEntityPropertyState getState();

	DfxEntityDataInstance *getSource();

	DfxEntity *getDfxEntity();

	DfxEntityProperty *getParent();

	bool isParentClean();

	void cleanParent();

	void dirtyParent();

	void deleteData();

	bool isFreeOnDeath();

	void setFreeOnDeath(bool);
};