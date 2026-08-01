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


#include "DfxAttributeTree.h"

DfxAttributeTree *DfxAttributeTree::_instance = nullptr;

DfxAttributeTree::DfxAttributeTree() {

}

DfxAttributeTree *DfxAttributeTree::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxAttributeTree;
	}

	return _instance;
}

void DfxAttributeTree::addAttribute(DfxAttribute *attr) {
	_attributes.push_back(attr);
}

void DfxAttributeTree::removeAttribute(DfxAttribute *attr) {
	auto itr = _attributes.begin();

	while (itr != _attributes.end()) {
		if ((*itr)->id() == attr->id()) {
			_attributes.erase(itr);
			break;
		}

		itr++;
	}
}

std::vector<DfxAttribute*> &DfxAttributeTree::attributes() {
	return _attributes;
}

DfxAttribute *DfxAttributeTree::_findAttribute(DfxAttribute *attr, std::queue<std::string> &q) {

	if (q.empty()) {
		return nullptr;
	}
	else {
		auto front = q.front();
		if (front == attr->name()) {
			q.pop();

			if (q.empty()) {
				return attr;
			}
			else {
				for (auto at : attr->attributes()) {
					auto rc = _findAttribute(at, q);
					
					if (rc != nullptr) {
						return rc;
					}
				}
			}
		}
		else{
			return nullptr;
		}
	}

	return nullptr;
}

DfxAttribute *DfxAttributeTree::findAttribute(std::string str) {

	fl::util::StringSplitter splitter(str);
	auto nameList = splitter.split('.');

	if (!nameList.empty()) {
		std::queue<std::string> qu;

		for (auto str : nameList) qu.push(str);

		for (auto p : _attributes) {
			auto attr = _findAttribute(p, qu);

			if (attr != nullptr) return attr;
		}
	}

	return nullptr;
}

DfxAttribute *DfxAttributeTree::findAttribute(size_t atid) {

	for (auto attr: _attributes) {
		if (attr->id() == atid) {
			return attr;
		}
	}

	return nullptr;
}