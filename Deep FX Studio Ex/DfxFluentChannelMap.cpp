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

#include "DfxFluentChannelMap.h"

DfxFluentChannelMap::DfxFluentChannelMap() {
	_size = 0;
}

DfxFluentChannelMap::~DfxFluentChannelMap() {
	removeAllAttributes();
}

size_t DfxFluentChannelMap::size() {
	return _size;
}

void DfxFluentChannelMap::resize(size_t sz) {

	if (sz > 0) {
		for (auto attr : _attributeMap) attr.second->resize(sz);
		_size = sz;
	}

}

size_t DfxFluentChannelMap::attributeType(std::string attrName) {

	auto itr = _attributeMap.find(attrName);

	if (itr != _attributeMap.end()) {
		return itr->second->type;
	}

	return 0xf4240;
}

DfxFluentChannel *DfxFluentChannelMap::channel(std::string attrName) {

	auto itr = _attributeMap.find(attrName);

	if (itr != _attributeMap.end()) {
		return itr->second;
	}

	return nullptr;
}

std::vector<DfxFluentChannel*> DfxFluentChannelMap::allChannels() {
	std::vector<DfxFluentChannel*> channels;

	for (auto p: _attributeMap) {
		channels.push_back(p.second);
	}

	return channels;
}

std::vector<std::pair<std::string, DfxFluentChannel*>> DfxFluentChannelMap::allAttributes() {
	std::vector<std::pair<std::string, DfxFluentChannel*>> output;

	for (auto p : _attributeMap) output.push_back(p);

	return output;
}

size_t DfxFluentChannelMap::attributeCount() {
	return _attributeMap.size();
}

bool DfxFluentChannelMap::removeAttribute(std::string attrName) {

	auto itr = _attributeMap.find(attrName);

	if (itr != _attributeMap.end()) {
		_attributeMap.erase(itr);
		return true;
	}

	return false;
}

void DfxFluentChannelMap::clear() {
	for (auto attr : _attributeMap) attr.second->clear();
}

void DfxFluentChannelMap::removeAllAttributes() {

	for (auto attr : _attributeMap) {
		attr.second->free_channel();
		delete attr.second;
	}

	_attributeMap.clear();
	_size = 0;
}

bool DfxFluentChannelMap::hasAttribute(std::string attrName) {
	auto itr = _attributeMap.find(attrName);

	if (itr != _attributeMap.end()) {
		return true;
	}

	return false;
}

