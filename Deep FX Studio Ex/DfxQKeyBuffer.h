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

#include <QKeySequence>
#include <vector>
#include <algorithm>
#include "DfxShortcutsDB.h"

class DfxQKeyBuffer{
private:
	std::vector<unsigned int> _keys;
public:
	void addKey(unsigned int key);
	void removeKey(unsigned int key);
	bool hasKey(unsigned int key);
	int  filter(int k);
	std::string keysToString();
	std::vector<unsigned int> &keys();
	std::vector<unsigned int> keys4();
	DfxShortcutsDB::Shortcut keysToShortcut();
};
