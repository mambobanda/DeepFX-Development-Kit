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

#include "DfxGeneratePointsHandler.h"

std::string DfxGeneratePointsHandler::model_id = "/POINT_CLOUD/GENERATE_POINTS_INFO";
size_t DfxGeneratePointsHandler::ID_GEN = 1;

DfxGeneratePointsHandler::DfxGeneratePointsHandler() {

}

DfxGeneratePointsHandler::~DfxGeneratePointsHandler() {

}

fl::Any *DfxGeneratePointsHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("generate_points")
		.setLabel("Generate Points" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		/// Global Properties
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "type", "Generator Type", (size_t)DfxGeneratePointsTool::GRID)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "reset", "Reset Point Cloud", true)

		/// Grid Properties
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "grid_count_x", "Grid Count X", (size_t)3)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "grid_count_y", "Grid Count Y", (size_t)3)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "grid_count_z", "Grid Count Z", (size_t)3)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_spacing_x", "Grid Spacing X", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_spacing_y", "Grid Spacing Y", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_spacing_z", "Grid Spacing Z", 1.f)

		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "grid_rand_seed", "Grid Random Seed", (size_t)50987)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_rand_pos_min_offset", "Grid Random Position Min Offset", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_rand_pos_max_offset", "Grid Random Position Max Offset", 0.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_rand_rot_offset", "Grid Random Rotation Offset", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "grid_rand_scl_offset", "Grid Random Scale Offset", 0.f)

		.build(space);
}

bool DfxGeneratePointsHandler::create() {
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
		DfxGlobal::console::println("Point Generator Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxGeneratePointsHandler::createToolNode() {
	
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

bool DfxGeneratePointsHandler::destroy() {
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

deepsg::SgToolNode *DfxGeneratePointsHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxGeneratePointsHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		deepsg::SgToolNode *gnode = new DfxGeneratePointsTool;

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

void DfxGeneratePointsHandler::deleteNode(deepsg::SgNode *node) {
	DfxGeneratePointsHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}