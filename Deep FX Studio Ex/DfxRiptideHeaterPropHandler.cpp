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

#include "DfxRiptideHeaterPropHandler.h"

std::string DfxRiptideHeaterPropHandler::model_id = "/RIPTIDE/HEATER_PROP";
size_t DfxRiptideHeaterPropHandler::ID_GEN = 1;

DfxRiptideHeaterPropHandler::DfxRiptideHeaterPropHandler() {
	_dataObject = nullptr;
}

DfxRiptideHeaterPropHandler::~DfxRiptideHeaterPropHandler() {

}

fl::Any *DfxRiptideHeaterPropHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("riptide_heater_prop")
		.setLabel("Riptide Heater" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "radius", "Radius", 1.f)

		.build(space);
}

bool DfxRiptideHeaterPropHandler::create() {
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

DfxRiptideHeaterPropNode *DfxRiptideHeaterPropHandler::propNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return (DfxRiptideHeaterPropNode*)any->rawData<DfxSimulationStagePropBase>();
	}
	else {
		return nullptr;
	}
}

bool DfxRiptideHeaterPropHandler::destroy() {

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
			DfxGlobal::console::println("Riptide Heater Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxRiptideHeaterPropNode *DfxRiptideHeaterPropHandler::createHeaterNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return (DfxRiptideHeaterPropNode*)any->rawData<DfxSimulationStagePropBase>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_prop_node", "Scene Graph Riptide Heater Prop Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxRiptideHeaterPropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxRiptideHeaterPropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_prop_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		DfxSimulationStagePropBase *gnode = new DfxRiptideHeaterPropNode(nullptr);
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

void DfxRiptideHeaterPropHandler::deleteNode(deepsg::SgNode *node) {

	DfxRiptideHeaterPropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}