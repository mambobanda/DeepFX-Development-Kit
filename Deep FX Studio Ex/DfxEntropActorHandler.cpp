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

#include "DfxEntropyActorHandler.h"


std::string DfxEntropyActorHandler::model_id = "/ENTROPY/ACTOR";
size_t DfxEntropyActorHandler::ID_GEN = 1;

DfxEntropyActorHandler::DfxEntropyActorHandler() {

}

DfxEntropyActorHandler::~DfxEntropyActorHandler() {

}

fl::Any *DfxEntropyActorHandler::createModel(fl::Space *space) {

	auto any = fl::DataObject::Builder()
		.setName("entropy_actor")
		.setLabel("Entropy Actor")
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "source_path", "Source Path", std::string(""))
		.addProperty(space, fl::type::INT, fl::role::NONE, "actor_type", "Actor Type", 0)

		.addProperty(space, fl::type::BOOL, fl::role::NONE, "constant_inflow", "Constant Inflow", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "constant_temperature", "Constant Temperature", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "constant_velocity", "Constant Inflow", false)

		.addProperty(space, fl::type::INT, fl::role::NONE, "inflow_rate_type", "Inflow Rate Type", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_rate", "Inflow Rate", 1.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_density", "Inflow Density", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_temperature", "Inflow Temperature", 500.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_velocity_x", "Inflow Velocity X", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_velocity_y", "Inflow Velocity Y", 0.0f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "inflow_velocity_z", "Inflow Velocity Z", 0.0f)

		.build(space);

	return any;
}

bool DfxEntropyActorHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_ACTORS");

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

DfxEntropyActorNode *DfxEntropyActorHandler::createActorNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

		if (any != nullptr) {
			return any->rawData<DfxEntropyActorNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto node = space->any("sg_stage_actor_node", "Entropy Actor Node");
			space->move(node, _dataObject);

			return (DfxEntropyActorNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

bool DfxEntropyActorHandler::destroy() {

	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		auto ndata00 = actorNode();

		if (ndata00 != nullptr) {

			((DfxSimulationStageBase*)ndata00->stage())->removeActor(ndata00->getID());
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");
			any->nullify();

		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxEntropyActorNode *DfxEntropyActorHandler::actorNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_actor_node");

	if (any != nullptr) {
		return any->rawData<DfxEntropyActorNode>();
	}
	else {
		return nullptr;
	}
}

deepsg::SgNode* DfxEntropyActorHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto snode = any->data<fl::DataObject>().property("sg_stage_actor_node");

		auto gnode = new DfxEntropyActorNode(nullptr);
		gnode->setReferencePath(referencePath);

		snode->newData<DfxEntropyActorNode>(DfxFluentTypes::SG_STAGE_ACTOR_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxEntropyActorHandler::deleteNode(deepsg::SgNode *node) {

	DfxEntropyActorHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}