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


class DfxColorField {
private:

	unsigned int _r, _g, _b;
	unsigned int _c : 24;

	inline void assignRGB() {
		_r = _c >> 16;
		_g = _c >> 8 & 0xff;
		_b = _c & 255;
	}

	inline void assignValue() {
		_c = (_r << 16 | _g << 8 | _b);
	}

public:
	DfxColorField(unsigned int r, unsigned int g, unsigned int b);
	DfxColorField(unsigned int value);
	virtual ~DfxColorField();

	unsigned int value();
	void setValue(unsigned int value);

	unsigned int r();
	unsigned int g();
	unsigned int b();

	void setRGB(unsigned int r, unsigned int g, unsigned int b);

	void increment();
	void decrement();
};