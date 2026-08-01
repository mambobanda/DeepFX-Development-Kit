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

#include "DfxServiceEntry.h"

DfxGlobalEntityName DfxServiceEntry::getUniqueName() {
	createService = nullptr;
	return uniquename;
}

void DfxServiceEntry::setUniqueName(DfxGlobalEntityName unq) {
	uniquename = unq;
}

unsigned int DfxServiceEntry::tagCount() {
	return (unsigned int)tags.size();
}

std::pair<std::string, std::string> DfxServiceEntry::getTag(unsigned int inx) {
	return tags[inx];
}

void DfxServiceEntry::addTag(std::string name, std::string content) {
	std::pair<std::string, std::string> p(name, content);
	tags.push_back(p);
}

void DfxServiceEntry::setCreate(DfxCreateService sfunc) {
	createService = sfunc;
}

void *DfxServiceEntry::newInstance() {

	if (createService != nullptr)
		return createService(uniquename);
	else
		return nullptr;
}