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

#include "SgNodeFilter.h"
#include "SgNode.h"

namespace deepsg {

	size_t SgNodeFilter::ID_GEN = 0xff;
	
	SgNodeFilter::SgNodeFilter() {
		_id = ID_GEN++;
		_node = nullptr;
	}

	SgNodeFilter::~SgNodeFilter() {
		if (_node != nullptr) {
			_node->removeNodeFilter(this);
		}
	}

	SgNode *SgNodeFilter::node() {
		return _node;
	}

	void SgNodeFilter::setNode(SgNode *nd) {
		_node = nd;
	}

	size_t SgNodeFilter::id() {
		return _id;
	}
	
}