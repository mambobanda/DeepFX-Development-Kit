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

#include "DfxSceneCleaner.h"
#include "DfxSimulationManager.h"

DfxSimulationManager *DfxSimulationManager::_instance = nullptr;

DfxSimulationManager::DfxSimulationManager() {
	_needsReset = false;
	_stepsCovered = 0;

	QObject::connect(DfxSceneCleaner::getInstance(), &DfxSceneCleaner::resetSignal, this, &DfxSimulationManager::reset);
}

DfxSimulationManager::~DfxSimulationManager() {

}

DfxSimulationManager *DfxSimulationManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxSimulationManager;
	}

	return _instance;
}

bool DfxSimulationManager::needsReset() {
	return _needsReset;
}

void DfxSimulationManager::forceReset(bool rs) {
	_needsReset = rs;

	emit resetTriggered();
}

int DfxSimulationManager::stepsCovered() {
	return _stepsCovered;
}

void DfxSimulationManager::setStepsCovered(int sc) {
	_stepsCovered = sc;
}

void DfxSimulationManager::resetSimulationState() {
	forceReset(true);
}

void DfxSimulationManager::run(DfxStageEvaluationInfo &info) {

	std::vector<DfxSimulationStageBase*> stageNodes;

	deepsg::SgTraversalFunction find_nodes_func = [&stageNodes](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->getType() == deepsg::SgNodeType::STAGE) {
			stageNodes.push_back((DfxSimulationStageBase*)node);
		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(find_nodes_func);

	for (auto stage: stageNodes) {
		if (stage->needsReset()) {
			stage->reset();
		}
	}
	
	for (auto stage : stageNodes) {
		if (stage->isSimulationEnabled()) {

			if (stage->doStep(info)) {

				stage->beginStep(info);
				stage->step(info);
				stage->endStep(info);

			}
		}
	}
	
}

void DfxSimulationManager::reset() {
	_stepsCovered = 0;
}