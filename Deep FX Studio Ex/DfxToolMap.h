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

#include "DfxToolMapNode.h"

class DfxToolMap {
private:

	static DfxToolMap *_instance;

	DfxToolMapNode *root;

	DfxToolMap();

public:

	static DfxToolMap *getInstance();

	std::vector<DfxToolMapNode*> domains();
	DfxToolMapNode *addDomain(std::string name, std::string resource, bool active = true);
};