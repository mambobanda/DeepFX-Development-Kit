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

#include <algorithm>
#include <vector>
#include <QStringList>
#include <QString>
#include <sstream>
#include <string>

class DfxIntegerTyple {
private:
	std::vector<int> tuple;
public:
	DfxIntegerTyple();
	DfxIntegerTyple(int a);
	DfxIntegerTyple(int a, int b);
	DfxIntegerTyple(int a, int b, int c);
	DfxIntegerTyple(int a, int b, int c, int d);
	DfxIntegerTyple(std::string);

	virtual ~DfxIntegerTyple();

	void add(int val);

	void remove(size_t inx);

	size_t count();

	int get(int inx);

	void clear();

	std::string toString();

	void fromString(std::string);
};