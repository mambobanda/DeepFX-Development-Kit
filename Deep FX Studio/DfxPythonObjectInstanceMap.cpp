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


#include "DfxPythonObjectInstanceMap.h"

std::unordered_map<size_t, py::object*>DfxPythonObjectInstanceMap::map;

void DfxPythonObjectInstanceMap::add(size_t id, py::object* obj) {
	std::pair< size_t, py::object*> p(id, obj);
	map.insert(p);
}

void DfxPythonObjectInstanceMap::removeAndDestroy(size_t id) {

	auto it = map.find(id);

	if (it != map.end()) {
		delete it->second;
		it->second = nullptr;
		map.erase(id);
	}
}