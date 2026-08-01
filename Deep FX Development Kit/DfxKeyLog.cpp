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

#include "DfxKeyLog.h"

DfxKeyLog::DfxKeyLog() {

}

DfxKeyLog::~DfxKeyLog() {

}

void DfxKeyLog::addKey(int key) {
	keys.insert(key);
}

void DfxKeyLog::removeKey(int key) {

	try {
		keys.erase(key);
	}
	catch (std::exception &ex) {
		DfxGlobal::console::println(ex.what());
	}
}

bool DfxKeyLog::hasKey(int key) {

	auto  it = std::find(keys.begin(), keys.end(), key);

	if (it != keys.end()) return true;
	else return false;

}