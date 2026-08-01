/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <vector>

class DfxEnum {
private:
	std::vector<std::string> enumerator;
	size_t currentIndex;
public:

	DfxEnum();

	void add(std::string name);

	std::string get(size_t inx);

	void setCurrentIndex(size_t inx);

	size_t getCurrentIndex();

	size_t size();
};