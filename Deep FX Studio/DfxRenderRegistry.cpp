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

#include "DfxRenderRegistry.h"

DfxRenderRegistry *DfxRenderRegistry::instance = nullptr;

DfxRenderRegistry::DfxRenderRegistry() {

}

DfxRenderRegistry *DfxRenderRegistry::getIntstance() {

	if (instance == nullptr) {
		instance = new DfxRenderRegistry();
	}

	return instance;
}

void DfxRenderRegistry::addProp(size_t uid, DfxRenderInterface *ptr, uint32_t type) {
	Prop pr;
	pr.itemPtr = ptr;
	pr.type = type;

	std::pair<size_t, Prop> p(uid, pr);
	propMap.insert(p);
}

void DfxRenderRegistry::removeProp(size_t uid) {

	auto pr = propMap.find(uid);

	if (pr != propMap.end()) {
		propMap.erase(uid);
	}
}

bool DfxRenderRegistry::hasProp(size_t uid) {

	auto pr = propMap.find(uid);

	if (pr != propMap.end()) {
		return true;
	}

	return false;
}

std::vector<DfxRenderInterface*> DfxRenderRegistry::findAllTypes(uint32_t type) {

	std::vector<DfxRenderInterface*> props;

	for (auto p : propMap) {

		if (p.second.type == type) {
			props.push_back(p.second.itemPtr);
		}
	}

	return props;
}

DfxRenderInterface* DfxRenderRegistry::find(size_t uid) {
	auto pr = propMap.find(uid);

	if (pr != propMap.end()) {
		return pr->second.itemPtr;
	}

	return nullptr;
}