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


#include <queue>
#include "DfxAttribute.h"
#include "FlUtil.h"

class DfxAttributeTree {
private:
	static DfxAttributeTree *_instance;

	std::vector<DfxAttribute*> _attributes;

	DfxAttribute *_findAttribute(DfxAttribute *attr, std::queue<std::string> &q);

	DfxAttributeTree();
public:

	static DfxAttributeTree *getInstance();

	void addAttribute(DfxAttribute *);
	void removeAttribute(DfxAttribute *);

	DfxAttribute *findAttribute(std::string);
	DfxAttribute *findAttribute(size_t);

	std::vector<DfxAttribute*> &attributes();

};