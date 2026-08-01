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


#include "DfxColorID.h"


DfxColorID::DfxColorID() {}

DfxColorID::DfxColorID(int _r, int _g, int _b) {

	ri = _r;
	gi = _g;
	bi = _b;

	rf = (float)_r / 255.f;
	gf = (float)_g / 255.f;
	bf = (float)_b / 255.f;

	rgb_str_i = std::to_string(ri) + std::to_string(gi) + std::to_string(bi);
	rgb_str_f = std::to_string(rf) + std::to_string(gf) + std::to_string(bf);
}

unsigned int DfxColorID::redi() const {
	return ri;
}

unsigned int DfxColorID::greeni() const {
	return gi;
}

unsigned int DfxColorID::bluei() const {
	return bi;
}

float DfxColorID::redf() const {
	return rf;
}

float DfxColorID::greenf() const {
	return gf;
}

float DfxColorID::bluef() const {
	return bf;
}

std::string DfxColorID::float_str() const {
	return rgb_str_f;
}

std::string DfxColorID::integer_str() const {
	return rgb_str_i;
}