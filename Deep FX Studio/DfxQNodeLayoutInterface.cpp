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

#include "DfxQNodeLayoutInterface.h"


DfxQNodeLayoutInterface::DfxQNodeLayoutInterface() {

}

DfxQNodeLayoutInterface::~DfxQNodeLayoutInterface() {

}

void DfxQNodeLayoutInterface::addNodeReference(DfxQBaseNode *nref) {
	size_t key = nref->getEntity()->getInstanceID();
	GNode gnode = {nref, .0f, .0f};
	gnodes.insert(std::pair<size_t, GNode>(key, gnode));
}

float DfxQNodeLayoutInterface::getNewX(size_t key) {

	auto it = gnodes.find(key);

	if (it != gnodes.end())
		return it->second.new_posx;
	else
		return 0.f;
}

float DfxQNodeLayoutInterface::getNewY(size_t key) {

	auto it = gnodes.find(key);

	if (it != gnodes.end())
		return it->second.new_posy;
	else
		return 0.f;
}