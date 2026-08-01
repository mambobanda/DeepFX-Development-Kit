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

#include "DfxMeshToGridHandler.h"

std::string DfxMeshToGridHandler::model_id = "/GRID/MESH_TO_GRID_INFO";
size_t DfxMeshToGridHandler::ID_GEN = 1;

DfxMeshToGridHandler::DfxMeshToGridHandler() {

}

DfxMeshToGridHandler::~DfxMeshToGridHandler() {

}

fl::Any *DfxMeshToGridHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("mesh_to_volume")
		.setLabel("Mesh To Volume" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "grid_name", "Grid Name", std::string("surface"))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "mesh_path", "Mesh Path", std::string(""))
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_size", "Voxel Size", 0.2f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "exterior_band", "Exterior Band", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "interior_band", "interior Band", 3.0f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "auto_sample_mesh", "Auto Sample Mesh", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "apply_transform", "Apply Transform", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "append", "Append", false)
		.build(space);

}

bool DfxMeshToGridHandler::create() {

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
		DfxGlobal::console::println("Mesh To Grid Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxMeshToGridHandler::createToolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_tool_node", "Scene Graph Mesh To Grid Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgToolNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxMeshToGridHandler::destroy() {
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
			DfxGlobal::console::println("Mesh To Grid Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

deepsg::SgToolNode *DfxMeshToGridHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode *DfxMeshToGridHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		DfxToolNode *gnode = new DfxMeshToGridTool;
		gnode->setReferencePath(referencePath);

		scenegraph_node->newData<DfxToolNode>(DfxFluentTypes::SG_TOOL_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxMeshToGridHandler::deleteNode(deepsg::SgNode *node) {
	DfxMeshToGridHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}