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

#include "SgStagePropCollectionNode.h"

namespace deepsg {

	std::string SgStagePropCollectionNode::_uniqueName = "UID_STAGE_PROP_COLLECTION_NODE";

	SgStagePropCollectionNode::SgStagePropCollectionNode(SgStageNode *stg) : SgNode(SgNodeType::STAGE_PROP_COLLECTION) {
		_stage = stg;
		setIsProtected(true);
		setIsMovable(false);
		setReferencePath("__PROP_COLLECTION__");
	}

	SgStagePropCollectionNode::~SgStagePropCollectionNode() {

	}

	SgStageNode *SgStagePropCollectionNode::stage() {
		return _stage;
	}

	std::string SgStagePropCollectionNode::uniqueName() {
		return _uniqueName;
	}

	std::string SgStagePropCollectionNode::getUniqueName() {
		return _uniqueName;
	}

};