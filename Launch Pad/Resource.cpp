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

#include "Resource.h"


size_t Resource::ID_GEN = 1000;

Resource::Resource() {
	_id = ++ID_GEN;
	_parent = nullptr;
}

Resource::~Resource() {

}

size_t Resource::id() {
	return _id;
}

std::vector<Resource*> &Resource::children() {
	return _children;
}

void Resource::addChild(Resource *chd) {
	chd->setParent(this);
	_children.push_back(chd);
}

void Resource::removeChild(size_t id) {
	auto itr = _children.begin();

	for (; itr != _children.end(); ++itr) {
		if ((*itr)->id() == id) {
			(*itr)->setParent(nullptr);
			_children.erase(itr);
			return;
		}
	}
}

Resource *Resource::findChildByIndex(int inx) {
	if (inx < _children.size())
		return _children[inx];
	else
		return nullptr;
}

Resource *Resource::findChildByID(size_t idn) {
	auto itr = _children.begin();

	for (; itr != _children.end(); ++itr) {
		if ((*itr)->id() == idn) {
			return (*itr);
		}
	}
	return nullptr;
}

Resource *Resource::parent() {
	return _parent;
}

void Resource::setParent(Resource *prt) {
	_parent = prt;
}

void Resource::unparent() {
	if (_parent != nullptr) {
		_parent->removeChild(_id);
	}
}

std::string Resource::name() {
	return _name;
}

void Resource::setName(std::string nm) {
	_name = nm;
}

int Resource::type() {
	return _type;
}

void Resource::setType(int typ) {
	_type = typ;
}

int Resource::role() {
	return _role;
}

void Resource::setRole(int role) {
	role = role;
}

int Resource::count() {
	if(_parent != nullptr) {
		return _parent->childCount(_id);
	}
	return 0;
}

int Resource::childCount(size_t id) {
	for (int i = 0; i < _children.size(); i++) {
		if (_children[i]->id() == id) return i;
	}

	return 0;
}