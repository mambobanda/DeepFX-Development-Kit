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

#include "DfxEntropyDomainPropHandler.h"

std::string DfxEntropyDomainPropHandler::model_id = "/ENTROPY/DOMAIN_PROP";
size_t DfxEntropyDomainPropHandler::ID_GEN = 1;

DfxEntropyDomainPropHandler::DfxEntropyDomainPropHandler() {
	_dataObject = nullptr;
}

DfxEntropyDomainPropHandler::~DfxEntropyDomainPropHandler() {

}

fl::Any *DfxEntropyDomainPropHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("entropy_domain_prop")
		.setLabel("Entropy Domain" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_size", "Voxel Size", 0.2f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "width", "Width", 50)
		.addProperty(space, fl::type::INT, fl::role::NONE, "height", "Height", 50)
		.addProperty(space, fl::type::INT, fl::role::NONE, "depth", "Depth", 50)

		.build(space);
}

bool DfxEntropyDomainPropHandler::create() {
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

DfxEntropyDomainPropNode *DfxEntropyDomainPropHandler::propNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return (DfxEntropyDomainPropNode*)any->rawData<DfxSimulationStagePropBase>();
	}
	else {
		return nullptr;
	}
}

bool DfxEntropyDomainPropHandler::destroy() {

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
			DfxGlobal::console::println("Entropy Domain Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxEntropyDomainPropNode *DfxEntropyDomainPropHandler::createDomainNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return (DfxEntropyDomainPropNode*)any->rawData<DfxSimulationStagePropBase>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_prop_node", "Scene Graph Entropy Domain Prop Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxEntropyDomainPropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxEntropyDomainPropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_prop_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		DfxSimulationStagePropBase *gnode = new DfxEntropyDomainPropNode(nullptr);
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

void DfxEntropyDomainPropHandler::deleteNode(deepsg::SgNode *node) {

	DfxEntropyDomainPropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}