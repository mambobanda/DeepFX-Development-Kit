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

#include "DfxPointProximityKillHandler.h"

std::string DfxPointProximityKillHandler::model_id = "/POINT_CLOUD/POINT_PROXIMITY_KILL_INFO";
size_t DfxPointProximityKillHandler::ID_GEN = 1;

DfxPointProximityKillHandler::DfxPointProximityKillHandler() {

}

DfxPointProximityKillHandler::~DfxPointProximityKillHandler() {

}

fl::Any *DfxPointProximityKillHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("points_proximity_kill")
		.setLabel("P-Cloud K-Proximity" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		/// Global Properties
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "live", "Live", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "accumulate", "Accumulate", false)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "reference", "Reference", std::string())
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "domain", "Domain", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "radius", "Radius", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "padding", "Padding", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "threshold", "Threshold", 100.f)

		.build(space);
}

bool DfxPointProximityKillHandler::create() {
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
		DfxGlobal::console::println("Point Proximity Kill Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgToolNode *DfxPointProximityKillHandler::createToolNode() {

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

bool DfxPointProximityKillHandler::destroy() {
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

deepsg::SgToolNode *DfxPointProximityKillHandler::toolNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_tool_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgToolNode>();
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxPointProximityKillHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_tool_node");

		deepsg::SgToolNode *gnode = new DfxPointProximityKillTool;

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

void DfxPointProximityKillHandler::deleteNode(deepsg::SgNode *node) {
	DfxPointProximityKillHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}