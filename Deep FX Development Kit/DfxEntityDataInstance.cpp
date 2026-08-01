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

#include "DfxEntityDataInstance.h"

DfxEntityDataInstance::DfxEntityDataInstance() {
	entity = nullptr;
	clean = false;
}

DfxEntityDataInstance::DfxEntityDataInstance(DfxEntity *ent) {
	entity = ent;
	clean = false;
}

DfxEntityDataInstance::~DfxEntityDataInstance() {

	for (auto p : instanceData)
		delete p.second;
}

DfxEntity *DfxEntityDataInstance::getDfxEntity() {
	return entity;
}

void DfxEntityDataInstance::insertProperty(size_t instance_id, DfxEntityProperty* prop) {
	std::pair< size_t, DfxEntityProperty*> p(instance_id, prop);
	instanceData.insert(p);
}

DfxEntityProperty* DfxEntityDataInstance::findProperty(size_t instance_id) {
	std::unordered_map<size_t, DfxEntityProperty*>::iterator it = instanceData.find(instance_id);

	if (it != instanceData.end()) {
		return it->second;
	}
	else {
		return nullptr;
	}
}

std::vector<size_t> DfxEntityDataInstance::getPropertyList() {
	std::vector<size_t> keys;

	for (std::pair<size_t, DfxEntityProperty*> k : instanceData) {
		keys.push_back(k.first);
	}

	return keys;
};

DfxEntityProperty *DfxEntityDataInstance::getPropertyInstance(size_t id) {
	return findProperty(id);
}

void *DfxEntityDataInstance::getProperty(size_t id) {
	return findProperty(id)->getData();
}

std::string DfxEntityDataInstance::getPropertyName(size_t id) {
	return findProperty(id)->getName();
}

size_t DfxEntityDataInstance::getPropertyType(size_t id) {
	return findProperty(id)->getType();
}

void DfxEntityDataInstance::addEventListener(DfxEntityDataListener evt) {
	while (!eventListenerLock.try_lock());

	eventListeners.push_back(evt);

	eventListenerLock.unlock();
}

void DfxEntityDataInstance::removeEventListener(DfxEntityDataListener evt) {
	while (!eventListenerLock.try_lock());

	unsigned int counter = 0;

	for (DfxEntityDataListener _evt : eventListeners) {
		if (_evt.getID() == evt.getID()) {
			eventListeners.erase(eventListeners.begin() + counter);
			break;
		}
		else {
			counter++;
		}
	}

	eventListenerLock.unlock();
}

void DfxEntityDataInstance::beginNotify() {
	while (!eventListenerLock.try_lock());

	for (DfxEntityDataListener _evt : eventListeners) {

		DfxEntityDataEvent evt_data(DfxEntityDataEventType::BEGIN_EVENT);
		
		std::thread(_evt, evt_data).detach();
	}

	eventListenerLock.unlock();
}

void DfxEntityDataInstance::endNotify() {
	while (!eventListenerLock.try_lock());

	for (DfxEntityDataListener _evt : eventListeners) {

		DfxEntityDataEvent evt_data(DfxEntityDataEventType::FINISHED_EVENT);
		
		std::thread(_evt, evt_data).detach();
	}

	eventListenerLock.unlock();
}

bool DfxEntityDataInstance::isClean() {
	return clean;
}

void DfxEntityDataInstance::makeClean() {
	clean = true;
}

void DfxEntityDataInstance::makeDirty() {
	clean = false;
}

