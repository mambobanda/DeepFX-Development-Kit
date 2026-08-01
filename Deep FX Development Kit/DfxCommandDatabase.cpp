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

#include "DfxCommandDatabase.h"

DfxCommandDatabase *DfxCommandDatabase::instance = nullptr;

DfxCommandDatabase::DfxCommandDatabase() {

}

DfxCommandDatabase *DfxCommandDatabase::getInstance() {
	if (instance == nullptr)
		instance = new DfxCommandDatabase;

	return instance;
}

bool DfxCommandDatabase::mapCommand(DfxString name, DfxCommandCreatePtr cmdptr) {

	std::unordered_map<std::string, DfxCommandCreatePtr>::iterator it = db.find(std::string(name.c_str()));

	if (it == db.end()) {

		db.insert(std::pair<std::string, DfxCommandCreatePtr>(std::string(name.c_str()), cmdptr));
		return true;
	}

	std::stringstream ss;
	ss << "Map Command ERROR: Command '" << name << "' already mapped.";

	DfxGlobal::console::println(ss.str());

	return false;
}

DfxCommandCreatePtr DfxCommandDatabase::findCommand(DfxString name) {

	std::unordered_map<std::string, DfxCommandCreatePtr>::iterator it = db.find(std::string(name.c_str()));

	if (it == db.end()) {
		return nullptr;
	}
	else {
		return it->second;
	}
}