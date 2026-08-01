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

#include "DfxEntropyCachePropHandler.h"

std::string DfxEntropyCachePropHandler::model_id = "/ENTROPY/CACHE_PROP";
size_t DfxEntropyCachePropHandler::ID_GEN = 1;

DfxEntropyCachePropHandler::DfxEntropyCachePropHandler() {
	_dataObject = nullptr;
}

DfxEntropyCachePropHandler::~DfxEntropyCachePropHandler() {

}

fl::Any *DfxEntropyCachePropHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("entropy_cache_prop")
		.setLabel("Entropy Cache" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_to_disk", "Save To Disk", false)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "filename", "Filename", std::string("cache"))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "directory", "Directory", std::string(""))

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "grid", "Grid Path", std::string(""))

		.build(space);
}

bool DfxEntropyCachePropHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_PROPS");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto object = createModel(space);

		/// Object XForm 
		auto xform = newXForm();

		/// Arrange properties into the right order
		space->move(object, temp_branch);
		space->move(xform, object);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failure
	return false;
}

DfxEntropyCachePropNode *DfxEntropyCachePropHandler::propNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return (DfxEntropyCachePropNode*)any->rawData<DfxSimulationStagePropBase>();
	}
	else {
		return nullptr;
	}
}

bool DfxEntropyCachePropHandler::destroy() {

	if (_dataObject != nullptr) {
		auto space = DfxSpace::getInstance();

		auto ndata00 = propNode();

		if (ndata00 != nullptr) {

			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Entropy Cache Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxEntropyCachePropNode *DfxEntropyCachePropHandler::createCacheNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return (DfxEntropyCachePropNode*)any->rawData<DfxSimulationStagePropBase>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_prop_node", "Scene Graph Entropy Cache Prop Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxEntropyCachePropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxEntropyCachePropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_prop_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		DfxSimulationStagePropBase *gnode = new DfxEntropyCachePropNode(nullptr);
		gnode->setReferencePath(referencePath);
		gnode->setXFormProperties(xform);

		scenegraph_node->newData<DfxSimulationStagePropBase>(DfxFluentTypes::SG_STAGE_PROP_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxEntropyCachePropHandler::deleteNode(deepsg::SgNode *node) {

	DfxEntropyCachePropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}