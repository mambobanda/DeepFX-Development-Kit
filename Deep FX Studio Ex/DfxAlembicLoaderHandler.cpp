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

#include "DfxAlembicLoaderHandler.h"

std::string DfxAlembicLoaderHandler::model_id = "/ALEMBIC/ALEMBIC_LOADER_INFO";
size_t DfxAlembicLoaderHandler::ID_GEN = 0;

DfxAlembicLoaderHandler::DfxAlembicLoaderHandler() {

}

DfxAlembicLoaderHandler::~DfxAlembicLoaderHandler() {

}

fl::Any *DfxAlembicLoaderHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("alembic_loader")
		.setLabel("Alembic Loader" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "file_path", "File Path", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "object_path", "Object Path", std::string(""))
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "scale", "Scale", 0.5f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "offset", "Offset", 0)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "apply_xform", "Apply Xform", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "animated", "Animated", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "loop", "Loop", false)
		.build(space);

}

bool DfxAlembicLoaderHandler::create() {

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
		DfxGlobal::console::println("Alembic Loader Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxAlembicLoaderHandler::createToolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_tool_node", "Scene Graph Alembic Loader Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgToolNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxAlembicLoaderHandler::destroy() {

	if (_dataObject != nullptr) {
		auto space = DfxSpace::getInstance();

		auto ndata00 = toolNode();

		if (ndata00 != nullptr) {

			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Object Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

deepsg::SgToolNode *DfxAlembicLoaderHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxAlembicLoaderHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		DfxAlembicLoaderTool *gnode = new DfxAlembicLoaderTool;
		gnode->setReferencePath(referencePath);

		scenegraph_node->newData<DfxAlembicLoaderTool>(DfxFluentTypes::SG_TOOL_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxAlembicLoaderHandler::deleteNode(deepsg::SgNode *node) {
	DfxAlembicLoaderHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}