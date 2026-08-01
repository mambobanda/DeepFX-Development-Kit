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

#include "DfxIntegerTuple.h"

DfxIntegerTyple::DfxIntegerTyple() {

}

DfxIntegerTyple::DfxIntegerTyple(int a) {
	tuple.push_back(a);
}

DfxIntegerTyple::DfxIntegerTyple(int a, int b) {
	tuple.push_back(a);
	tuple.push_back(b);
}

DfxIntegerTyple::DfxIntegerTyple(int a, int b, int c) {
	tuple.push_back(a);
	tuple.push_back(b);
	tuple.push_back(c);
}

DfxIntegerTyple::DfxIntegerTyple(int a, int b, int c, int d) {
	tuple.push_back(a);
	tuple.push_back(b);
	tuple.push_back(c);
	tuple.push_back(d);
}

DfxIntegerTyple::DfxIntegerTyple(std::string str) {
	fromString(str);
}

DfxIntegerTyple::~DfxIntegerTyple() {

}

void DfxIntegerTyple::add(int val) {
	tuple.push_back(val);
}

void DfxIntegerTyple::remove(size_t inx) {
	tuple.erase(tuple.begin() + inx);
}

size_t DfxIntegerTyple::count() {
	return tuple.size();
}

int DfxIntegerTyple::get(int inx) {
	return tuple[inx];
}

void DfxIntegerTyple::clear() {
	tuple.clear();
}

std::string DfxIntegerTyple::toString() {
	std::stringstream ss;

	for (auto v : tuple) {
		ss << v << ";";
	}

	return ss.str();
}

void DfxIntegerTyple::fromString(std::string str) {
	QString s(str.c_str());

	QStringList split = s.split(";");

	for (auto s : split) {
		int val = s.toInt();
		tuple.push_back(val);
	}
}