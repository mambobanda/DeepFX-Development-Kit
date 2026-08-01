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

#include "DfxEntityProperty.h"

DfxEntityProperty::DfxEntityProperty(size_t id, DfxEntityPropertyType *type, std::string name, DfxEntityDataInstance *source) {
	
	this->id = id;
	this->name = name;
	this->type = type->getID();

	data = type->create();

	state = DfxEntityPropertyState::NORMAL_STATE;

	this->source = source;

	parentClean = true;
	freeOnDeath = true;

	parent = nullptr;
}

DfxEntityProperty::~DfxEntityProperty() {
	if (freeOnDeath) {
		delete data;
	}
}

void *DfxEntityProperty::getData() {

	if (parent == nullptr) {
		return (void*)(data);
	}
	else {
		return parent->getData();
	}
}

size_t DfxEntityProperty::getID() {
	return id;
}

void DfxEntityProperty::addEventListener(DfxEntityDataListener evt) {
	while (!eventListenerLock.try_lock());

	eventListeners.push_back(evt);

	eventListenerLock.unlock();
}

void DfxEntityProperty::removeEventListener(DfxEntityDataListener evt) {
	while (!eventListenerLock.try_lock());

	unsigned int counter = 0;

	for (DfxEntityDataListener _evt: eventListeners) {
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


void DfxEntityProperty::beginNotify() {
	while (!eventListenerLock.try_lock());

	for (DfxEntityDataListener _evt : eventListeners) {

		DfxEntityDataEvent evt_data(DfxEntityDataEventType::BEGIN_EVENT);
		
		std::thread(_evt, evt_data).detach();
	}

	eventListenerLock.unlock();
}

void DfxEntityProperty::endNotify() {
	while (!eventListenerLock.try_lock());

	for (DfxEntityDataListener _evt : eventListeners) {

		DfxEntityDataEvent evt_data(DfxEntityDataEventType::FINISHED_EVENT);

		std::thread(_evt, evt_data).detach();
	}

	eventListenerLock.unlock();
}

std::string  DfxEntityProperty::getName() {
	return name;
}

size_t  DfxEntityProperty::getType() {
	return type;
}

DfxEntityDataInstance *DfxEntityProperty::getSource() {
	return source;
}

DfxEntity *DfxEntityProperty::getDfxEntity() {
	if(parent == nullptr)
		return source->getDfxEntity();
	else {
		return parent->getDfxEntity();
	}
}

void DfxEntityProperty::setState(DfxEntityPropertyState s) {
	state = s;
}

bool DfxEntityProperty::hasBinding() {
	return parent == nullptr ? false : true;
}

bool DfxEntityProperty::isBindingClean() {
	if (parent == nullptr) {
		return true;
	}
	else {
		return parent->getSource()->getDfxEntity()->branchIsClean();
	}
}

void DfxEntityProperty::bind(DfxEntityProperty *bnd) {

	if (state == DfxEntityPropertyState::INPUT_STATE) {

		if (bnd->getState() == DfxEntityPropertyState::OUTPUT_STATE) {

			DfxEntity *entinst = bnd->getDfxEntity();

			DfxEntity *query = entinst->queryChild(getDfxEntity()->getInstanceID());

			if (query == nullptr)
				entinst->addChild(getDfxEntity());

			parent = bnd;
		}
	}
	else throw DfxEntityPropertyException("Failed to bind property, incorrect state.");
}

void DfxEntityProperty::unbind() {
	
	if (state == DfxEntityPropertyState::INPUT_STATE) {

		if (parent != nullptr) {
			bool noLongerChild = true;

			std::vector<size_t> plist = source->getPropertyList();

			for (auto pid : plist) {
				if (pid == id) continue;

				DfxEntityProperty *pinst = source->getPropertyInstance(pid);

				if (pinst == nullptr) throw DfxEntityPropertyException("Failed to unbind property, DfxEntityProperty* access violation.");

				if (pinst->getState() == DfxEntityPropertyState::INPUT_STATE &&
					pinst->hasBinding()) {


					/*DfxEntity *check = parent->getDfxEntity()->queryChild(pinst->getDfxEntity()->getInstanceID());
					if (check != nullptr) {
						noLongerChild = false;
						break;
					}*/

					if (pinst->getParent()->getDfxEntity()->getInstanceID() ==
						parent->getDfxEntity()->getInstanceID()) {
							noLongerChild = false;
							break;
					}
				}
				
			}

			if (noLongerChild) {
				parent->getDfxEntity()->removeChild(source->getDfxEntity());
			}

			parent = nullptr;
		}
	}
}

DfxEntityPropertyState DfxEntityProperty::getState() {
	return state;
}

DfxEntityProperty *DfxEntityProperty::getParent() {
	return parent;
}

bool DfxEntityProperty::isParentClean() {
	return parentClean;
}

void DfxEntityProperty::cleanParent() {
	parentClean = true;
}

void DfxEntityProperty::dirtyParent() {
	parentClean = false;
}

void DfxEntityProperty::deleteData() {
	delete data;
	data = nullptr;
}

void DfxEntityProperty::setData(void *d) {
	data = d;
}

bool DfxEntityProperty::isFreeOnDeath() {
	return freeOnDeath;
}

void DfxEntityProperty::setFreeOnDeath(bool val) {
	freeOnDeath = val;
}