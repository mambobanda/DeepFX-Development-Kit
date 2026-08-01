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

#include "DfxGroupHandler.h"

std::string DfxGroupHandler::model_id = "/GROUP/GROUP_INFO";
size_t DfxGroupHandler::ID_GEN = 1;

DfxGroupHandler::DfxGroupHandler() {

}

DfxGroupHandler::~DfxGroupHandler() {

}


fl::Any *DfxGroupHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto object = fl::DataObject::Builder()
		.setName("group")
		.setLabel("Group" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.build(space);

	return object;
}

bool DfxGroupHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/GROUPS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, temp_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failier
	return false;
}

bool DfxGroupHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto ndata00 = groupNode();

		if (ndata00 != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_group_node");
			any->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

DfxGroupNode *DfxGroupHandler::groupNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_group_node");

		if (any != nullptr) {
			return any->rawData<DfxGroupNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_group_node", "Scene Graph Group Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxGroupNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}


deepsg::SgNode* DfxGroupHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_group_node");

		auto gnode = new DfxGroupNode;

		scenegraph_node->newData<DfxGroupNode>(DfxFluentTypes::SG_GROUP_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxGroupHandler::deleteNode(deepsg::SgNode *node) {
	DfxGroupHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}