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

#include "SgStageNode.h"

namespace deepsg {

	SgStageNode::SgStageNode(size_t stageT): SgNode(SgNodeType::STAGE) {

		_stageType = stageT;

		_actorCollection = new SgStageActorCollectionNode(this);
		_propCollection  = new SgStagePropCollectionNode(this);

		addChild(_actorCollection);
		addChild(_propCollection);
	}

	SgStageNode::~SgStageNode() {
		removeChild(_actorCollection);
		removeChild(_propCollection);

		delete _actorCollection;
		delete _propCollection;
	}
	
	std::vector<SgStageActorNode*> SgStageNode::actors() {
		std::vector<SgStageActorNode*> output;

		for (auto actor: _actorCollection->children()) {
			if (actor->getType() == SgNodeType::STAGE_ACTOR) {
				output.push_back((SgStageActorNode*)actor);
			}
		}

		return output;
	}

	std::vector<SgStagePropNode*> SgStageNode::props() {
		std::vector<SgStagePropNode*> output;

		for (auto prop : _propCollection->children()) {
			if (prop->getType() == SgNodeType::STAGE_PROP) {
				output.push_back((SgStagePropNode*)prop);
			}
		}

		return output;
	}

	size_t SgStageNode::stageType() {
		return _stageType;
	}

	SgStageActorCollectionNode *SgStageNode::actorCollectionNode() {
		return _actorCollection;
	}

	SgStagePropCollectionNode  *SgStageNode::propCollectionNode() {
		return _propCollection;
	}
};