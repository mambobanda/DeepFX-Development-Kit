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

#include "DfxSimulationStageCommon.h"


namespace DfxSimulationStageCommon {
	
	void propActorDeleteNode(deepsg::SgNode *node) {
		// Do Nothing
	}

	deepsg::SgNode* loadPropActorNode(std::string referencePath, deepsg::SgNode *parent) {

		if (referencePath == "__ACTOR_COLLECTION__") {
			if (parent->getType() == deepsg::SgNodeType::STAGE) {
				return ((deepsg::SgStageNode*)parent)->actorCollectionNode();
			}
		}

		if (referencePath == "__PROP_COLLECTION__") {
			if (parent->getType() == deepsg::SgNodeType::STAGE) {
				return ((deepsg::SgStageNode*)parent)->propCollectionNode();
			}
		}

		return nullptr;
	}
}