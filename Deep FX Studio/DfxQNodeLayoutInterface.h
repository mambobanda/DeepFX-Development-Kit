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

#include <unordered_map>
#include "DfxQBaseNode.h"

class DfxQNodeLayoutInterface {
protected:
	struct GNode {
		DfxQBaseNode *baseNode;
		float new_posx;
		float new_posy;
	};
protected:
	std::unordered_map<size_t, GNode> gnodes;
public:

	DfxQNodeLayoutInterface();
	virtual ~DfxQNodeLayoutInterface();

	virtual void solveLayout() = 0;

	void addNodeReference(DfxQBaseNode *);
	
	float getNewX(size_t);
	float getNewY(size_t);
};