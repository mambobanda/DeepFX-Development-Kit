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

#include "DfxFluentCleaner.h"

void DfxFluentCleaner::resetSimulations() {
	if (DfxFluentSimplexCollection::getInstance()->size() > 0) {
		std::vector<DfxSimulationStageBase*> stageNodes;

		deepsg::SgTraversalFunction find_nodes_func = [&stageNodes](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			if (node->getType() == deepsg::SgNodeType::STAGE) {
				stageNodes.push_back((DfxSimulationStageBase*)node);
			}
		};

		deepsg::SgRootNode::getInstance()->traverseScene(find_nodes_func);

		auto uids = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid");

		if (uids != nullptr) {
			for (auto stage : stageNodes) {
				stage->clearFluent();
			}
		}
	}
}

void DfxFluentCleaner::cleanAll() {

	DfxFluentSetManager::getInstance()->clear();

	DfxFluentSimplexCollection::getInstance()->clearAllObjects();
	DfxFluentSimplexCollection::getInstance()->clearAttributeMap();

	DfxFluentStreamCollection::getInstance()->clearAttributeMap();

}