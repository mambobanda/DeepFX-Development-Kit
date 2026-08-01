/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include "SgGridNode.h"

namespace deepsg {

	SgGridNode::SgGridNode(): SgNode(SgNodeType::GRID) {
		_interface = nullptr;
	}

	SgGridNode::~SgGridNode() {

	}

	void SgGridNode::setGridAdapter(SgGridAdapterInterface *adp) {
		_interface = adp;
	}

	SgGridAdapterInterface *SgGridNode::getGridAdapter() {
		return _interface;
	}

}