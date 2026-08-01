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

class DfxColorCounter {
private:
	unsigned int red;
	unsigned int green;
	unsigned int blue;

public:

	DfxColorCounter();

	void increment();

	unsigned int redi();

	unsigned int greeni();

	unsigned int bluei();

	float redf();

	float greenf();

	float bluef();
};
