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

#include "DfxObjectLoaderService.h"


DfxObjectLoaderService::DfxObjectLoaderService(DfxGlobalEntityName uniquename):DfxEntity(uniquename) {

}

DfxObjectLoaderService::~DfxObjectLoaderService() {

}

void DfxObjectLoaderService::compute() { };

void DfxObjectLoaderService::onDataChanged(DfxEntityDataInstance *instance) { };

void DfxObjectLoaderService::setFilename(std::string str) {
	*((std::string*)getProperty(filename)) = str;
}

std::string DfxObjectLoaderService::getFilename() {
	return *((std::string*)getProperty(filename));
}

int DfxObjectLoaderService::getStatus() {
	return *((size_t*)getProperty(status));
}

void DfxObjectLoaderService::setStatus(size_t st) {
	*((size_t*)getProperty(status)) = st;
}

bool  DfxObjectLoaderService::hasLoader(std::string tag) {
	
	auto entries = DfxServiceRegistry::fetchServiceEntries(DFX_OBJECTLOADER_SERVICE);

	for (auto entry : entries) {

		for (size_t inx = 0; inx < entry->tagCount(); inx++) {

			if (std::string("format").compare(entry->getTag(inx).first) == 0) {

				if (tag.compare(entry->getTag(inx).second) == 0) {
					return true;
				}
			}
		}
	}

	return false;
}

DfxObjectLoaderService*  DfxObjectLoaderService::fetchLoader(std::string tag) {

	auto entries = DfxServiceRegistry::fetchServiceEntries(DFX_OBJECTLOADER_SERVICE);

	for (auto entry : entries) {

		for (size_t inx = 0; inx < entry->tagCount(); inx++) {

			if (std::string("format").compare(entry->getTag(inx).first) == 0) {

				if (tag.compare(entry->getTag(inx).second) == 0) {
					return (DfxObjectLoaderService*)entry->newInstance();
				}
			}
		}
	}

	return nullptr;
}

void DfxObjectLoaderService::initialize() {

	DfxEntityDB::createProperty(filename, "filename", new DfxEntityPropertyTypes::StringPropertyType());
	DfxEntityDB::createProperty(status, "status", new DfxEntityPropertyTypes::Size_TPropertyType());
}