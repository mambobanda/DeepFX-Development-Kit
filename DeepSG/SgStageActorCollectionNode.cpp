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

#include "SgStageActorCollectionNode.h"

namespace deepsg {

	std::string SgStageActorCollectionNode::_uniqueName = "UID_STAGE_ACTOR_COLLECTION_NODE";

	SgStageActorCollectionNode::SgStageActorCollectionNode(SgStageNode *stg): SgNode(SgNodeType::STAGE_ACTOR_COLLECTION) {
		_stage = stg;
		setIsProtected(true);
		setIsMovable(false);
		setReferencePath("__ACTOR_COLLECTION__");
	}

	SgStageActorCollectionNode::~SgStageActorCollectionNode() {

	}

	SgStageNode *SgStageActorCollectionNode::stage() {
		return _stage;
	}

	std::string SgStageActorCollectionNode::uniqueName() {
		return _uniqueName;
	}
	
	std::string SgStageActorCollectionNode::getUniqueName() {
		return _uniqueName;
	}

};