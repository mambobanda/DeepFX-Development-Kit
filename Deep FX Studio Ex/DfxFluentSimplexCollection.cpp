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

#include "DfxFluentSimplexCollection.h"

DfxFluentSimplexCollection *DfxFluentSimplexCollection::_instance = nullptr;


DfxFluentSimplexCollection::DfxFluentSimplexCollection() {
	initAttributeMap();
}

DfxFluentSimplexCollection::~DfxFluentSimplexCollection() {

}

void DfxFluentSimplexCollection::initAttributeMap() {
	_attributeMap.addAttribute<size_t>("@uid");
	_attributeMap.addAttribute<glm::vec3>("@position");
	_attributeMap.addAttribute<std::string>("@set_name");
	_attributeMap.addAttribute<std::string>("@scene_path");
	_attributeMap.addAttribute<std::string>("@alias");
	_attributeMap.addAttribute<glm::mat4>("@xform");
}

DfxFluentSimplexCollection *DfxFluentSimplexCollection::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxFluentSimplexCollection();
	}

	return _instance;
}

std::vector<DfxFluentSimplexObject*> &DfxFluentSimplexCollection::simplexObjects() {
	return _simplexObjects;
}

size_t DfxFluentSimplexCollection::addObject(DfxFluentSimplexObject* obj, std::string groupid, std::string scene_path, glm::mat4 xform) {
	std::lock_guard<std::mutex> guard(lock);

	auto index = _simplexObjects.size();

	_simplexObjects.push_back(obj);
	_attributeMap.resize(_simplexObjects.size());

	_attributeMap.at<size_t>("@uid")[index] = DfxFluentBigID::id();
	_attributeMap.at<glm::vec3>("@position")[index] = obj->center();
	_attributeMap.at<std::string>("@set_name")[index] = groupid;
	_attributeMap.at<std::string>("@scene_path")[index] = scene_path;
	_attributeMap.at<std::string>("@alias")[index] = "##";
	_attributeMap.at<glm::mat4>("@xform")[index] = xform;
	
	return index;
}

DfxFluentChannelMap &DfxFluentSimplexCollection::atrributeMap() {
	return _attributeMap;
}

size_t DfxFluentSimplexCollection::size() {
	return _simplexObjects.size();
}

bool DfxFluentSimplexCollection::immutableAttribute(std::string attri) {
	if (attri == "@uid" ||
		attri == "@position" ||
		attri == "@set_name" ||
		attri == "@scene_path") {
		return true;
	}

	return false;
}

void DfxFluentSimplexCollection::clearAllObjects() {
	for (auto sobj : _simplexObjects) delete sobj;
	_simplexObjects.clear();
}

void DfxFluentSimplexCollection::clearAttributeMap() {
	_attributeMap.removeAllAttributes();
	initAttributeMap();
}