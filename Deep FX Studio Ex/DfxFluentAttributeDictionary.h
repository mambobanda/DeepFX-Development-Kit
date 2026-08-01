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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <vector>
#include <string>
#include <algorithm>

class DfxFluentAttributeDictionary {
private:
	static DfxFluentAttributeDictionary *_instance;

	std::vector<std::string> _atrributeList;

	DfxFluentAttributeDictionary();
public:

	static DfxFluentAttributeDictionary *getInstance();

	std::vector<std::string> &attributeList();
	void addAttribute(std::string);
	void sortList();
};