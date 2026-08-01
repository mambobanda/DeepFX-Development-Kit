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

#include "DfxGraphEditorChannel.h"

DfxGraphEditorChannel::DfxGraphEditorChannel() {
	_parent = nullptr;
}

DfxGraphEditorChannel::~DfxGraphEditorChannel() {

}

DfxGraphEditorChannel *DfxGraphEditorChannel::parent() {
	return _parent;
}

void DfxGraphEditorChannel::setParent(DfxGraphEditorChannel * prt) {
	_parent = prt;
}

std::vector<DfxGraphEditorChannel*> &DfxGraphEditorChannel::channels() {
	return _channels;
}

DfxGraphEditorChannel *DfxGraphEditorChannel::findChannelByIndex(int index) {
	return _channels[index];
}

void DfxGraphEditorChannel::addChannel(DfxGraphEditorChannel* channel) {
	channel->setParent(this);
	_channels.push_back(channel);
}

void DfxGraphEditorChannel::removeChannel(std::string pth) {

	for (auto itr = _channels.begin(); itr != _channels.end(); itr++) {
		if ((*itr)->path() == pth) {
			delete (*itr);
			_channels.erase(itr);
			return;
		}
	}

}

void DfxGraphEditorChannel::removeAllChannels(std::string pth) {

	for (auto itr = _channels.begin(); itr != _channels.end(); itr++) {
		if ((*itr)->path() == pth) {
			(*itr)->clear();
			delete (*itr);
			_channels.erase(itr);
			return;
		}
	}

}

void DfxGraphEditorChannel::clear() {

	for (auto itr = _channels.begin(); itr != _channels.end(); itr++) {
		(*itr)->clear();
		delete (*itr);
	}

	_channels.clear();

}

int DfxGraphEditorChannel::channelCount() {
	return _channels.size();
}

int DfxGraphEditorChannel::index(std::string pth) {

	int counter = 0;
	for (auto itr = _channels.begin(); itr != _channels.end(); itr++) {
		if ((*itr)->path() == pth) {
			return counter;
		}
		counter++;
	}

	return 0;

}

int DfxGraphEditorChannel::relativeIndex() {
	
	if (_parent != nullptr) {
		_parent->index(_path);
	}

	return 0;
}

void DfxGraphEditorChannel::setPath(std::string pth) {
	_path = pth;
}

std::string DfxGraphEditorChannel::path() {
	return _path;
}

void DfxGraphEditorChannel::setColor(QColor clr) {
	_color = clr;
}

QColor DfxGraphEditorChannel::color() {
	return _color;
}

void DfxGraphEditorChannel::setType(int typ) {
	_type = typ;
}

int DfxGraphEditorChannel::type() {
	return _type;
}

std::string DfxGraphEditorChannel::name() {
	return _name;
}

void DfxGraphEditorChannel::setName(std::string name) {
	_name = name;
}