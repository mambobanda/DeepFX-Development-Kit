/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "DfxOpenVDBGridAdapter.h"


DfxOpenVDBGridAdapter::DfxOpenVDBGridAdapter() {
	grid_type = DfxGridTypes::OPEN_VDB;
}

DfxOpenVDBGridAdapter::~DfxOpenVDBGridAdapter() {
	clearAll();
}

void DfxOpenVDBGridAdapter::addChannel(deepsg::SgGridChannel *chn) {
	_channels.push_back(chn);
}

void DfxOpenVDBGridAdapter::removeChannel(unsigned index) {
	_channels.erase(_channels.begin() + index);
}

void DfxOpenVDBGridAdapter::removeChannel(std::string channelName) {
	auto itr = _channels.begin();

	while(itr != _channels.end()) {
		if ((*itr)->name() == channelName) {
			delete (*itr);
			itr = _channels.erase(itr);
		}
	}

}

void DfxOpenVDBGridAdapter::clearAll() {
	auto itr = _channels.begin();

	while (itr != _channels.end()) {
		delete (*itr);
		itr = _channels.erase(itr);
	}
}

void DfxOpenVDBGridAdapter::clear() {
	clearAll();
}

unsigned int DfxOpenVDBGridAdapter::gridImplementation() {
	return grid_type;
}

unsigned int DfxOpenVDBGridAdapter::channelCount() {
	return _channels.size();
}

std::string DfxOpenVDBGridAdapter::channelName(unsigned int index) {
	return _channels[index]->name();
}

deepsg::SgGridChannel *DfxOpenVDBGridAdapter::channel(unsigned index) {
	return _channels[index];
}

deepsg::SgGridChannel *DfxOpenVDBGridAdapter::channel(std::string channelName) {
	auto itr = _channels.begin();

	while (itr != _channels.end()) {
		if ((*itr)->name() == channelName) {
			return (*itr);
		}
	}

	return nullptr;
}