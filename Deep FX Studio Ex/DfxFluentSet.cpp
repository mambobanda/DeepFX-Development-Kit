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

#include "DfxFluentSet.h"
#include "DfxFluentStreamCollection.h"
#include "DfxFluentSimplexCollection.h"


DfxFluentSet::DfxFluentSet(uint8_t typ) {
	_parent = nullptr;
	_type = typ;
	_visible = true;
	_color = glm::vec3(1.f, 0.f, 0.f);
	_referenceType = NORMAL_SET;
	set_id = DfxFluentBigID::id();
}

DfxFluentSet::~DfxFluentSet() { }

size_t DfxFluentSet::id() {
	return set_id;
}

uint8_t DfxFluentSet::type() {
	return _type;
}

void DfxFluentSet::setType(uint8_t typ) {
	_type = typ;
}

void DfxFluentSet::forceSoftSet() {
	_referenceType = SOFT_SET;
}

bool DfxFluentSet::isSoftSet() {
	return _referenceType == SOFT_SET;
}

std::vector<size_t> &DfxFluentSet::indices() {
	return _indices;
}

void DfxFluentSet::addIndices(std::vector<size_t> &inputs) {

	std::string *set_name = nullptr;
	if (!isSoftSet() && type() != EMPTY) {
		
		if (type() == SIMPLEX) {
			set_name = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@set_name");
		}
		else if (type() == STREAM) {
			set_name = DfxFluentStreamCollection::getInstance()->atrributeMap().at<std::string>("@set_name");
		}
	}

	for (auto ind : inputs) {
		_indices.push_back(ind);

		if (set_name != nullptr) {
			set_name[ind] = name();
		}
	}
}

void DfxFluentSet::removeIndex(size_t inx) {
	auto itr = _indices.begin();

	while (itr != _indices.end()) {
		if ((*itr) == inx) {
			itr = _indices.erase(itr);
		}
		else {
			itr++;
		}
	}
}

void DfxFluentSet::removeIndices(std::vector<size_t> &inputs) {
	std::vector<size_t> newIndices;
	for (auto ind00 : _indices) {
		bool skip = false;

		for (auto ind01 : inputs) {
			if (ind00 == ind01) skip = true;
		}

		if (!skip) newIndices.push_back(ind00);
	}
}

bool DfxFluentSet::isVisible() {
	return _visible;
}

void DfxFluentSet::setIsVisible(bool viz) {
	_visible = viz;
}

bool DfxFluentSet::hasObjectFromScene(std::string path) {

	if (DfxFluentSimplexCollection::getInstance()->atrributeMap().size() > 0) {
		auto spathAttributes = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@scene_path");
		for (auto i : _indices) {
			if (!spathAttributes[i].empty() && spathAttributes[i] == path) return true;
		}
	}

	return false;
}

DfxFluentSet *DfxFluentSet::parent() {
	return _parent;
}

void DfxFluentSet::setParent(DfxFluentSet *prt) {
	_parent = prt;
}

bool DfxFluentSet::moveByIndex(size_t index, DfxFluentSet *otherset) {
	auto itr = _indices.begin();

	while (itr != _indices.end()) {

		if ((*itr) == index) {
			otherset->addIndices(std::vector<size_t> { (*itr) });
			_indices.erase(itr);
			return true;
		}

		itr++;
	}

	return false;
}

std::vector<DfxFluentSet*> &DfxFluentSet::children() {
	return _children;
}

void DfxFluentSet::addChild(DfxFluentSet* set) {
	set->setParent(this);
	_children.push_back(set);
}

DfxFluentSet* DfxFluentSet::findChild(size_t uid) {
	auto itr = _children.begin();

	while (itr != _children.end()) {
		if ((*itr)->id() == uid) {
			return (*itr);
		}
		itr++;
	}

	return nullptr;
}

size_t DfxFluentSet::relativeIndex() {
	size_t counter = 0;
	
	if (_parent != nullptr) {
		for (auto &val : _parent->children()) {
			if (val->id() == id()) return counter;
			counter++;
		}
	}

	return 0;
}

void DfxFluentSet::removeChild(size_t uid) {
	auto itr = _children.begin();

	while (itr != _children.end()) {
		if ((*itr)->id() == uid) {
			(*itr)->setParent(nullptr);
			_children.erase(itr);
			return;
		}
		itr++;
	}
}

void DfxFluentSet::unparent() {
	if (_parent != nullptr) {
		_parent->removeChild(this->id());
	}

	setParent(nullptr);
}

glm::vec3 DfxFluentSet::color() {
	return _color;
}

void DfxFluentSet::setColor(glm::vec3 color) {
	_color = color;
}

std::string DfxFluentSet::name() {
	return _name;
}

void DfxFluentSet::setName(std::string nm) {
	_name = nm;
}