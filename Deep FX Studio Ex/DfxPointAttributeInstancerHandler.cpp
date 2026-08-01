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

#include "DfxPointAttributeInstancerHandler.h"

std::string DfxPointAttributeInstancerHandler::model_id = "/POINT_CLOUD/POINT_ATTRIBUTE_INSTANCER_INFO";
size_t DfxPointAttributeInstancerHandler::ID_GEN = 1;

DfxPointAttributeInstancerHandler::DfxPointAttributeInstancerHandler() {

}

DfxPointAttributeInstancerHandler::~DfxPointAttributeInstancerHandler() {

}

fl::Any *DfxPointAttributeInstancerHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("point_attribute_instancer")
		.setLabel("Point Attribute Instancer" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable", "Enable", true)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_mode", "Render Mode", 1)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "limit", "Limit", 0)
		.build(space);
}

bool DfxPointAttributeInstancerHandler::create() {
	// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/TOOLS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {

		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, object_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}
	else {
		DfxGlobal::console::println("Point Attribute Instancer: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxPointAttributeInstancerHandler::createToolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_tool_node", "Scene Graph Tool Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgToolNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxPointAttributeInstancerHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto nodeAny = _dataObject->data<fl::DataObject>().property("sg_tool_node");
		auto node = toolNode();

		if (node != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(node);
			delete node;
			nodeAny->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

deepsg::SgToolNode *DfxPointAttributeInstancerHandler::toolNode() {
	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxPointAttributeInstancerHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		deepsg::SgToolNode *gnode = new DfxPointAttributeInstancerTool;

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgToolNode>(DfxFluentTypes::SG_TOOL_NODE, gnode);

		/// Finish setting up scene graph node
		gnode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxPointAttributeInstancerHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointAttributeInstancerHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}