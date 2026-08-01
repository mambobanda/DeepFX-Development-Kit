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

#include "DfxPointShapeHandler.h"

std::string DfxPointShapeHandler::model_id = "/POINT_CLOUD/POINT_SHAPE_INFO";
size_t DfxPointShapeHandler::ID_GEN = 1;

DfxPointShapeHandler::DfxPointShapeHandler() {

}

DfxPointShapeHandler::~DfxPointShapeHandler() {

}

fl::Any *DfxPointShapeHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("points_shape")
		.setLabel("Point Shape" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		/// Global Properties
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "group", "Group", std::string(""))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "distribution", "Distribution", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "seed", "Seed", 495848)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "uniform_scale_min", "Uniform Scale Min", 100.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "uniform_scale_max", "Uniform Scale Max", 100.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_offset_x", "Min Offset X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_offset_y", "Min Offset Y", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_offset_z", "Min Offset Z", 0.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_offset_x", "Max Offset X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_offset_y", "Max Offset Y", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_offset_z", "Max Offset Z", 0.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_scale_x", "Min Scale X", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_scale_y", "Min Scale Y", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_scale_z", "Min Scale Z", 1.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_scale_x", "Max Scale X", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_scale_y", "Max Scale Y", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_scale_z", "Max Scale Z", 1.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_rotation_x", "Min Rotation X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_rotation_y", "Min Rotation Y", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_rotation_z", "Min Rotation Z", 0.0f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_rotation_x", "Max Rotation X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "max_rotation_y", "Max Rotation Y", 0.0f)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "max_rotation_z", "Max Rotation Z", 0.0f)
		
		.build(space);
}

bool DfxPointShapeHandler::create() {
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
		DfxGlobal::console::println("Point Shape Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxPointShapeHandler::createToolNode() {

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

bool DfxPointShapeHandler::destroy() {
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

deepsg::SgToolNode *DfxPointShapeHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxPointShapeHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		deepsg::SgToolNode *gnode = new DfxPointShapeTool;

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

void DfxPointShapeHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointShapeHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}