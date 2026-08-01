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

#include "DfxColorCounter.h"

DfxColorCounter::DfxColorCounter() {
	red = 0;
	green = 0;
	blue = 1;
}

void DfxColorCounter::increment() {

	if (blue < 254) {

		blue++;
	}
	else {
		if (green < 254) {

			green++;
			blue = 0;
		}
		else {

			if (red < 254) {

				red++;
				green = 0;
				blue = 0;
			}
			else {

				red = 0;
				green = 0;
				blue = 0;
			}
		}
	}
}

unsigned int DfxColorCounter::redi() {
	return red;
}

unsigned int DfxColorCounter::greeni() {
	return green;
}

unsigned int DfxColorCounter::bluei() {
	return blue;
}

float DfxColorCounter::redf() {
	return (float)red / 255.f;
}

float DfxColorCounter::greenf() {
	return (float)green / 255.f;
}

float DfxColorCounter::bluef() {
	return (float)blue / 255.f;
}