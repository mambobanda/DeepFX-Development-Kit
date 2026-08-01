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

#include <sstream>
#include "DfxServiceRegistry.h"
#include "DfxGlobal.h"

std::unordered_map<size_t, DfxServiceClass*> DfxServiceRegistry::services;

void DfxServiceRegistry::registerSerivce(size_t class_id, DfxServiceEntry *entry) {
		
	for (auto service : services) {
		if ( service.second->unique_id == class_id ) {
			service.second->services.push_back(entry);
		}
	}
}

std::vector<DfxServiceEntry*> DfxServiceRegistry::fetchServiceEntries(size_t class_id) {
	for (auto service : services) {

		if (service.second->unique_id == class_id) {
			return service.second->services;
		}
	}

	return std::vector<DfxServiceEntry*>();
}

