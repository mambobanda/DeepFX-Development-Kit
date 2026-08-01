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

#include "DfxPluginDatabase.h"

std::unordered_map<unsigned int, std::vector<dfxPluginReference*>>  DfxPluginDatabase::db;

std::string DfxPluginDatabase::currentDLLPath = "N/A";

void DfxPluginDatabase::addPlugin(unsigned int catagory, dfxPluginReference* ref) {

	ref->path = _strdup(currentDLLPath.c_str());

	std::unordered_map<unsigned int, std::vector<dfxPluginReference*>>::iterator it = db.find(catagory);

	if (it == db.end()) {
		std::unordered_map<unsigned int, std::vector<dfxPluginReference*>>::iterator it2 = db.find(0);

		if (it2 == db.end()) {
			std::vector<dfxPluginReference*> arr;
			arr.push_back(ref);
			db.insert(std::pair<unsigned int, std::vector<dfxPluginReference*>>(0, arr));
		}
		else {
			it2->second.push_back(ref);
		}
	}
	else {
		it->second.push_back(ref);
	}
}

void DfxPluginDatabase::setPath(std::string p) {
	currentDLLPath = p;
}

std::vector<dfxPluginReference*> DfxPluginDatabase::getCatagoryList(unsigned int catagory) {
	std::vector<dfxPluginReference*> list;

	std::unordered_map<unsigned int, std::vector<dfxPluginReference*>>::iterator it = db.find(catagory);

	if (it != db.end()) {
		list = it->second;
	}

	return list;
}