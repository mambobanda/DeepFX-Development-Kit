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

#include "DfxRiptideCachePropHandler.h"
#include "DfxRiptideCachePropNode.h"

std::string DfxRiptideCachePropHandler::model_id = "/RIPTIDE/CACHE_PROP";
size_t DfxRiptideCachePropHandler::ID_GEN = 1;

DfxRiptideCachePropHandler::DfxRiptideCachePropHandler() {

}

DfxRiptideCachePropHandler::~DfxRiptideCachePropHandler() {

}

fl::Any *DfxRiptideCachePropHandler::createModel(fl::Space *space) {

	auto any = fl::DataObject::Builder()
		.setName("riptide_cache_prop")
		.setLabel("Cache")
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "location", "Save Location", 0)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "filename", "Filename", std::string("Default Cache"))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "directory", "Directory", std::string(""))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "format", "Format", 0)

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "point_cloud", "Point Cloud Path", std::string(""))
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "preview_particle_size", "Preview Particle Size", 4.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "peak_preview_velocity", "Peak Preview Velocity", 10.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "secondary_color_r", "Secondary Color R", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "secondary_color_g", "Secondary Color G", 1.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "secondary_color_b", "Secondary Color B", 1.0f)

		.build(space);

	return any;
}

bool DfxRiptideCachePropHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_PROPS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		size_t id_g = ID_GEN++;
		std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

		/// Object Reference
		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, temp_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failure
	return false;
}

DfxRiptideCachePropNode *DfxRiptideCachePropHandler::createCacheNode() {
	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return any->rawData<DfxRiptideCachePropNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto node = space->any("sg_stage_prop_node", "Deep Rising Cache Prop");
			space->move(node, _dataObject);

			return (DfxRiptideCachePropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxRiptideCachePropHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		auto ndata00 = cacheNode();

		if (ndata00 != nullptr) {

			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;
			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");
			any->nullify();

		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxRiptideCachePropNode *DfxRiptideCachePropHandler::cacheNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return any->rawData<DfxRiptideCachePropNode>();
	}
	else {
		return nullptr;
	}
}

deepsg::SgNode* DfxRiptideCachePropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto snode = any->data<fl::DataObject>().property("sg_stage_prop_node");

		DfxRiptideCachePropNode *gnode = new DfxRiptideCachePropNode(nullptr);
		gnode->setReferencePath(referencePath);

		snode->newData<DfxRiptideCachePropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxRiptideCachePropHandler::deleteNode(deepsg::SgNode *node) {
	DfxRiptideCachePropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}