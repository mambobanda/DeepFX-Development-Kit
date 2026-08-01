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

#include "DfxGlobalProperties.h"

DfxGlobalProperties *DfxGlobalProperties::instance = nullptr;

DfxGlobalProperties::DfxGlobalProperties() {

}

DfxGlobalProperties *DfxGlobalProperties::getInstance() {

	if (instance == nullptr) {
		instance = new DfxGlobalProperties();
	}

	return instance;
}

bool DfxGlobalProperties::hasProperty(std::string path,  DfxPropertyID propertyID) {

	auto it = globals.find(path);

	if (it != globals.end()) {

		for (auto entry : it->second) {
			if (entry->propertyID == propertyID) {
				return true;
			}
		}
	}

	return false;
}

DfxGlobalProperties::GlobalProperty *DfxGlobalProperties::getProperty(std::string path, DfxPropertyID propertyID) {
	auto it = globals.find(path);

	if (it != globals.end()) {

		for (auto entry : it->second) {
			if (entry->propertyID == propertyID) {	
				return entry;
			}
		}
	}

	return nullptr;
}

DfxGlobalProperties::GlobalProperty *DfxGlobalProperties::addProperty(std::string path, size_t instanceID, DfxPropertyID propertyID) {

	GlobalProperty *gp = new GlobalProperty;
	gp->instanceID = instanceID;
	gp->propertyID = propertyID;

	auto it = globals.find(path);

	if (it != globals.end()) {

		for (auto entry : it->second) {
			if (entry->instanceID == instanceID && entry->propertyID == propertyID) {
				delete gp;
				return entry;
			}
		}

		it->second.push_back(gp);

		return gp;
	}
	else {
		std::vector<GlobalProperty*> props;
		props.push_back(gp);

		std::pair<std::string, std::vector<GlobalProperty*>> p(path, props);
		globals.insert(p);

		return gp;
	}


}

void DfxGlobalProperties::removeProperty(std::string path, size_t instanceID, DfxPropertyID propertyID) {

	auto it = globals.find(path);

	if (it != globals.end()) {

		for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) {
			if ((*iter)->instanceID == instanceID && (*iter)->propertyID == propertyID) {
				delete (*iter);
				it->second.erase(iter);
				return;
			}
		}
	}
}

void DfxGlobalProperties::removeGlobal(std::string path) {

	auto it = globals.begin();

	while (it != globals.end()){
		
		if (path.compare(it->first) == 0) {

			for (auto p : it->second) {
				delete p;
			}

			globals.erase(it);
			return;
		}

		++it;
	}
}