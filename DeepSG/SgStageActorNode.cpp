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

#include "SgStageActorNode.h"

namespace deepsg {

	SgStageActorNode::SgStageActorNode(SgStageNode *stg, size_t typ): SgNode(SgNodeType::STAGE_ACTOR) {
		_stage = stg;
		_type = typ;
	}

	SgStageActorNode::~SgStageActorNode() {

	}
	
	SgStageNode *SgStageActorNode::stage() {
		return _stage;
	}

	void SgStageActorNode::setStage(SgStageNode *stage) {
		_stage = stage;
	}

	size_t SgStageActorNode::actorType() {
		return _type;
	}
};