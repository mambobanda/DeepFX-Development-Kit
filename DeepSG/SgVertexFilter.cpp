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

#include "SgVertexFilter.h"
#include "SgGeoNode.h"

namespace deepsg {

	size_t SgVertexFilter::ID_GEN = 0xff;

	SgVertexFilter::SgVertexFilter() {
		_id = ID_GEN++;
		_node = nullptr;
	}

	SgVertexFilter::~SgVertexFilter() {
		if (_node != nullptr) {
			_node->removeVertexFilter(this);
		}
	}

	SgGeoNode *SgVertexFilter::node() {
		return _node;
	}

	void SgVertexFilter::setNode(SgGeoNode *nd) {
		_node = nd;
	}

	size_t SgVertexFilter::id() {
		return _id;
	}


}