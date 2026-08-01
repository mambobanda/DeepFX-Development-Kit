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

#pragma once

#include <vector> 
#include <DfxEntityDB.h>
#include <DfxEntityBasicPropertyTypes.h>

class DfxGroupDBEntry {
public:
	size_t uid;
};

class DfxGroupDB {
private:
	static DfxGroupDB *instance;
	std::vector<DfxGroupDBEntry> entries;

	DfxGroupDB();
public:

	~DfxGroupDB();

	static DfxGroupDB *getInstance();

	void addEntry(DfxGroupDBEntry );
	void removeEntry(size_t);

	size_t numberOfGroups();

	DfxGroupDBEntry findEntryByName(std::string);

	DfxGroupDBEntry findEntryByUID(size_t);
};