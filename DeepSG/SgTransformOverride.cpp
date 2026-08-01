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

#include "SgTransformOverride.h"
#include "SgNode.h"

namespace deepsg {

	size_t SgTransformOverride::ID_GEN = 0xff;

	SgTransformOverride::SgTransformOverride(SgNode *prt) {
		_parent = prt;
		_visibility = PRIVATE;
	}

	SgTransformOverride::~SgTransformOverride() {
		if (parent() != nullptr) {
			parent()->removeTransformOverride(this);
		}
	}

	size_t SgTransformOverride::id() {
		return _id;
	}

	SgNode *SgTransformOverride::parent() {
		return _parent;
	}

	void SgTransformOverride::setParent(SgNode *node) {
		_parent = node;
	}

	unsigned int SgTransformOverride::visibility() {
		return _visibility;
	}

	void SgTransformOverride::setVisibility(unsigned int vis) {
		_visibility = vis;
	}
}