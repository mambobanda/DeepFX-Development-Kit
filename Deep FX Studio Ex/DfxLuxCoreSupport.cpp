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

#include "DfxLuxCoreSupport.h"

LuxUV::LuxUV(float _u, float _v)
	: u(_u), v(_v) {
}

LuxUV::LuxUV(const float v[2]) : u(v[0]), v(v[1]) {
}

LuxPoint::LuxPoint(float _x, float _y, float _z )
	: x(_x), y(_y), z(_z) {
}

LuxPoint::LuxPoint(const float v[3]) : x(v[0]), y(v[1]), z(v[2]) {
}

LuxTriangle::LuxTriangle() { }

LuxTriangle::LuxTriangle(const unsigned int v0, const unsigned int v1, const unsigned int v2) {
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
}

LuxItemID::LuxItemID() {
	reset();
}

size_t LuxItemID::getID() {
	return base++;
}

void LuxItemID::setBase(size_t sb) {
	base = sb;
}

void LuxItemID::reset() {
	base = 0;
}