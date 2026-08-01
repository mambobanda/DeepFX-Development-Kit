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

#include <DfxGlobal.h>
#include <DfxSpace.h>
#include <DfxQSceneManager.h>
#include <DfxUpdater.h>
#include "DfxExternalToolGlobalState.h"

GlobalState::Space *GlobalState::space() {
	return &_space;
}

GlobalState::SceneGraph *GlobalState::sceneGraph() {
	return &_sgraph;
}

// Global
void GlobalState::g_sceneUpdated() {
	DfxUpdater::getInstance()->sceneUpdated();
}

// Space
fl::Any *GlobalState::spc_findAbsolute(std::string path) {
	return DfxSpace::getInstance()->findAbsolute(path);
}

void GlobalState::spc_remove(fl::Any *any) {
	DfxSpace::getInstance()->remove(any);
}

void GlobalState::spc_move(fl::Any *any, fl::Any *dest, fl::SpaceObserver *source) {
	DfxSpace::getInstance()->move(any, dest, source);
}

fl::Any *GlobalState::spc_any(std::string name, std::string label) {
	return DfxSpace::getInstance()->any(name, label);
}

fl::Space *GlobalState::spc_root() {
	return DfxSpace::getInstance();
}

// SceneGraph
void GlobalState::sg_moveNode(deepsg::SgNode *node, deepsg::SgNode *to) {
	DfxQSceneManager::getInstance()->moveNode(node, to);
}

void GlobalState::sg_removeNode(deepsg::SgNode *node) {
	DfxQSceneManager::getInstance()->removeNode(node);
}

deepsg::SgNode *GlobalState::sg_rootNode() {
	return deepsg::SgRootNode::getInstance();
}

GlobalState *GlobalState::createGlobalState() {

	auto state = new GlobalState;
	
	// I/O & Commands
	state->println = DfxGlobal::console::println;
	state->exec = DfxGlobal::command::exec;
	state->messageBox = DfxGlobal::info::messageBox;
	state->sceneUpdated = g_sceneUpdated;

	// Space
	state->_space.findAbsolute = spc_findAbsolute;
	state->_space.remove = spc_remove;
	state->_space.move = spc_move;
	state->_space.any  = spc_any;
	state->_space.root = spc_root;

	// SceneGraph
	state->_sgraph.moveNode = sg_moveNode;
	state->_sgraph.removeNode = sg_removeNode;
	state->_sgraph.rootNode = sg_rootNode;

	return state;
}