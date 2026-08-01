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

#include "DfxAttribute.h"


unsigned int DfxAttribute::UID_GEN = 100;

DfxAttribute::DfxAttribute() {
	_id = ++UID_GEN;
	_parent = nullptr;
	_role = ROOT;
	_path = "";
}

DfxAttribute::~DfxAttribute() {
	deleteChildren();
}

unsigned int DfxAttribute::id() {
	return _id;
}

size_t DfxAttribute::attributeCount() {
	return _attributes.size();
}

DfxAttribute *DfxAttribute::attribute(size_t inx) {
	return _attributes[inx];
}

std::vector<DfxAttribute*> &DfxAttribute::attributes() {
	return _attributes;
}

unsigned int DfxAttribute::role() {
	return _role;
}

void DfxAttribute::setRole(unsigned int rl) {
	_role = rl;
}

DfxAttribute *DfxAttribute::parent() {
	return _parent;
}

void DfxAttribute::setParent(DfxAttribute *pt) {
	_parent = pt;
}

unsigned int DfxAttribute::count(unsigned int ct) {

	for (size_t inx = 0; inx < _attributes.size(); inx++) {
		if (ct == _attributes[inx]->id()) return inx;
	}

	return 0;
}

unsigned int DfxAttribute::count() {
	if (_parent != nullptr) return _parent->count(_id);

	return 0;
}

std::string DfxAttribute::path() {
	return _path;
}

void DfxAttribute::setPath(std::string pth) {
	_path = pth;
}

void DfxAttribute::addAttribute(DfxAttribute *attr) {
	attr->setParent(this);
	_attributes.push_back(attr);
}

void DfxAttribute::removeAttribute(DfxAttribute *attr) {
	auto itr = _attributes.begin();

	while (itr != _attributes.end()) {

		if ((*itr)->id() == attr->id()) {
			attr->setParent(nullptr);
			_attributes.erase(itr);
			break;
		}

		itr++;
	}
}

std::string DfxAttribute::attributePath() {
	std::vector<std::string> names;
	auto attr = this;

	while (true) {
		if (attr->parent() == nullptr) {
			names.insert(names.begin(), attr->name());
			break;
		}
		else {
			names.insert(names.begin(), attr->name());
			attr = attr->parent();
		}
	}

	std::stringstream ss;
	for (size_t inx = 0; inx < names.size(); inx++) {
		ss << names[inx];

		if ((names.size() - 1) != inx) {
			ss << ".";
		}
	}

	return ss.str();
}

void DfxAttribute::deleteChildren() {

	for (auto chd : _attributes) {
		chd->deleteChildren();
		delete chd;
	}
}