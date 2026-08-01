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

#include "DfxGroupDB.h"
#include "DfxGroupNode.h"

DfxGroupDB *DfxGroupDB::instance = nullptr;

DfxGroupDB::DfxGroupDB() {

}

DfxGroupDB::~DfxGroupDB() {

}

DfxGroupDB *DfxGroupDB::getInstance() {

	if (instance == nullptr) {
		instance = new DfxGroupDB;
	}

	return instance;
}

void DfxGroupDB::addEntry(DfxGroupDBEntry entr) {
	entries.push_back(entr);
}

void DfxGroupDB::removeEntry(size_t uid) {

	size_t counter = 0;
	for (int inx = 0; inx < entries.size(); inx++) {

		if (entries[inx].uid == uid) {
			entries.erase(entries.begin() + counter);
			return;
		}

		counter++;
	}
}

size_t DfxGroupDB::numberOfGroups() {
	return entries.size();
}

DfxGroupDBEntry DfxGroupDB::findEntryByName(std::string str) {
	using namespace DfxEntityPropertyTypes;

	DfxGroupDBEntry rc;
	rc.uid = -1;

	for (auto ent : entries) {
		std::string name = *(std::string*)DfxEntityDB::getDataInstance(ent.uid)->getProperty(DfxNodes::DfxGroupNode::inName);
		if (name.compare(str) == 0) return ent;
	}

	return rc;
}

DfxGroupDBEntry DfxGroupDB::findEntryByUID(size_t uid) {
	DfxGroupDBEntry rc;
	rc.uid = -1;

	for (auto ent : entries) {
		if (ent.uid == uid) return ent;
	}

	return rc;
}