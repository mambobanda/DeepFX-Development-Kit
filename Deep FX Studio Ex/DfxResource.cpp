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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxResource.h"
#include "DfxResourcePtr.h"

size_t DfxResource::ID_GEN = 0x3e8;

DfxResource::DfxResource() {
	_id = ID_GEN++;

	_mode = resource::mode::NONE;
	_type = resource::type::NONE;
	_path = "";
	_format = resource::format::NONE;

	_referenced = false;
	_embedded = true;

	_shareCounter = 0;

	_releaseCallback = nullptr;

	_parent = nullptr;
}

DfxResource::DfxResource(unsigned int nType, unsigned int rType, std::string path) {
	_id = ID_GEN++;

	_mode = nType;
	_type = rType;
	_path = path;
	_format = resource::format::NONE;

	_referenced = false;
	_embedded = true;
	
	_shareCounter = 0;

	_releaseCallback = nullptr;

	_parent = nullptr;
}

size_t DfxResource::maxId(DfxResource *rsc) {
	size_t output = rsc->id();

	for (auto child: rsc->children()) {
		output = std::max(output, maxId(child));
	}

	return output;
}

size_t DfxResource::maxId() {
	return maxId(this);
}

void DfxResource::setBaseId(size_t uid) {
	ID_GEN = uid;
}

size_t DfxResource::id() {
	return _id;
}

void DfxResource::setId(size_t uid) {
	_id = uid;
}

std::vector<DfxResource*> DfxResource::children() {
	return _children;
}

bool DfxResource::hasParent() {
	return _parent != nullptr;
}

DfxResource *DfxResource::parent() {
	return _parent;
}

void DfxResource::setParent(DfxResource *prt) {
	_parent = prt;
}

unsigned int DfxResource::mode() {
	return _mode;
}

void DfxResource::setMode(unsigned int nt) {
	_mode = nt;
}

unsigned int DfxResource::format() {
	return _format;
}

void DfxResource::setFormat(unsigned int fmt) {
	_format = fmt;
}

unsigned int DfxResource::type() {
	return _type;
}

void DfxResource::setType(unsigned int rts) {
	_type = rts;
}

std::string DfxResource::name() {
	return _name;
}

void  DfxResource::setName(std::string nm) {
	_name = nm;
}

std::string DfxResource::path() {
	return _path;
}

std::string DfxResource::setPath(std::string nm) {
	return _path = nm;
}

void *DfxResource::resource() {
	return _resource;
}

void DfxResource::setResource(void *rc) {
	_resource = rc;
}

size_t DfxResource::childCount(size_t chd) {

	size_t counter = 0;

	for (auto n : _children) {

		if (n->id() == chd) return counter;

		counter++;
	}

	return 0;
}

size_t DfxResource::count() {

	if (_parent != nullptr) {
		return _parent->childCount(id());
	}

	return 0;
}

void DfxResource::addChild(DfxResource *rnode) {
	rnode->setParent(this);
	_children.push_back(rnode);
}

DfxResource *DfxResource::getChild(size_t inx) {
	
	if (!_children.empty()) {
		if (inx < _children.size()) {
			return _children[inx];
		}
	}

	return nullptr;
}

DfxResource *DfxResource::getChildByID(size_t uid) {
	auto itr = _children.begin();

	while (itr != _children.end()) {

		if ((*itr)->id() == uid) {
			return (*itr);
		}

		itr++;
	}

	return nullptr;
}

void DfxResource::removeChild(DfxResource *rnode) {
	removeChildByID(rnode->id());
}

void DfxResource::removeChild(size_t inx) {
	auto itr = _children.begin();

	int counter = 0;
	while (itr != _children.end()) {

		if (counter == inx) {
			(*itr)->setParent(nullptr);
			_children.erase(itr);
			return;
		}

		counter++;
		itr++;
	}
}

void DfxResource::removeChildByID(size_t uid) {
	auto itr = _children.begin();

	while (itr != _children.end()) {

		if ((*itr)->id() == uid) {
			_children.erase(itr);
			return;
		}

		itr++;
	}
}

void DfxResource::detach() {
	if (parent() != nullptr) {
		parent()->removeChild(this);

		for (auto p : _pointers) {
			p->_ready = false;
			p->_rptr = nullptr;
		}
	}
}

DfxResourcePtr *DfxResource::newPointer() {

	auto ptr = new DfxResourcePtr;
	ptr->_rptr = this;
	ptr->_ready = true;
	
	_pointers.push_back(ptr);

	_shareCounter++;

	return ptr;
}

void DfxResource::setReleaseCallback(resource::ReleaseCallback cb) {
	_releaseCallback = cb;
}

void DfxResource::releasePointer(DfxResourcePtr *ptr) {

	auto itr = _pointers.begin();

	while (itr != _pointers.end()) {

		if ((*itr)->id() == ptr->id()) {

			ptr->_ready = false;
			_pointers.erase(itr);
			_shareCounter--;
			return;
		}

		itr++;
	}
}

void DfxResource::releasePointer(DfxResource *resource, DfxResourcePtr *resourcePtr, bool autoDelete) {

	resource->releasePointer(resourcePtr);

	if (autoDelete) {
		if (resource->resourceCount() == 0) {
			releaseResource(resource);
		}
	}
}

void DfxResource::releaseResource(DfxResource *resource) {
	resource->detach();
	auto rCallback = resource->releaseCallback();
	if (rCallback != nullptr) rCallback(resource->resource());
	delete resource;
}

resource::ReleaseCallback DfxResource::releaseCallback() {
	return _releaseCallback;
}

size_t DfxResource::resourceCount() {
	return _shareCounter;
}

size_t DfxResource::childCount() {
	return _children.size();
}

bool DfxResource::isReferenced() {
	return _referenced;
}

void DfxResource::setIsReferenced(bool val) {
	_referenced = val;
}

bool DfxResource::isEmbedded() {
	return _embedded;
}

void DfxResource::setEmbedded(bool val) {
	_embedded = val;
}