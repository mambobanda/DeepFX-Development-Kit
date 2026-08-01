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

#include <exception>
#include <set>
#include <algorithm>
#include "DfxGlobal.h"

class DfxKeyLog {
private:
	std::set<int> keys;
public:

	DfxKeyLog();
	~DfxKeyLog();

	void addKey(int key);
	void removeKey(int key);
	bool hasKey(int key);
};
