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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxFluentAttributeDictionary.h"

DfxFluentAttributeDictionary *DfxFluentAttributeDictionary::_instance = nullptr;

DfxFluentAttributeDictionary::DfxFluentAttributeDictionary() {

}

DfxFluentAttributeDictionary *DfxFluentAttributeDictionary::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxFluentAttributeDictionary;
	}

	return _instance;
}

std::vector<std::string> &DfxFluentAttributeDictionary::attributeList() {
	return _atrributeList;
}

void DfxFluentAttributeDictionary::addAttribute(std::string str) {
	_atrributeList.push_back(str);
}
	
void DfxFluentAttributeDictionary::sortList() {
	std::sort(_atrributeList.begin(), _atrributeList.end());
}