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

class LuxUV {
public:
	LuxUV(float _u = 0.f, float _v = 0.f);

	LuxUV(const float v[2]);

	float u, v;
};

class LuxPoint {
public:
	LuxPoint(float _x = 0.f, float _y = 0.f, float _z = 0.f);

	LuxPoint(const float v[3]);

	float x, y, z;
};

class LuxTriangle {
public:
	LuxTriangle();
	LuxTriangle(const unsigned int v0, const unsigned int v1, const unsigned int v2);

	unsigned int v[3];
};


class LuxItemID {
private:
	size_t base;
public:
	LuxItemID();
	size_t getID();
	void setBase(size_t);
	void reset();
};