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

#include "DfxFluentStreamCollection.h"

DfxFluentStreamCollection *DfxFluentStreamCollection::_instance = nullptr;


DfxFluentStreamCollection::DfxFluentStreamCollection() {
	initAttributeMap();
}

DfxFluentStreamCollection::~DfxFluentStreamCollection() {

}

size_t DfxFluentStreamCollection::growCollection(size_t amount) {

	if (amount > 0) {

		size_t offset = size();
		_attributeMap.resize(offset + amount);

		auto uid = _attributeMap.at<size_t>("@uid");
		for (size_t inx = offset; inx < amount; inx++) uid[inx] = DfxFluentBigID::id();

		return offset;
	}

	return 0;
}

void DfxFluentStreamCollection::initAttributeMap() {
	_attributeMap.addAttribute<size_t>("@uid");
	_attributeMap.addAttribute<glm::vec3>("@position");
	_attributeMap.addAttribute<std::string>("@set_name");
	_attributeMap.addAttribute<glm::mat4>("@xform");
}

DfxFluentStreamCollection *DfxFluentStreamCollection::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxFluentStreamCollection();
	}

	return _instance;
}

DfxFluentChannelMap &DfxFluentStreamCollection::atrributeMap() {
	return _attributeMap;
}

size_t DfxFluentStreamCollection::size() {
	return _attributeMap.size();
}

bool DfxFluentStreamCollection::immutableAttribute(std::string attri) {

	if (attri == "@uid" ||
		attri == "@position" ||
		attri == "@set_name" ) {
		return true;
	}

	return false;
}

void DfxFluentStreamCollection::clearAttributeMap() {
	_attributeMap.removeAllAttributes();
	initAttributeMap();
}