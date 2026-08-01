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

#include "DfxEntropyStageHandler.h"


std::string DfxEntropyStageHandler::model_id = "/SIMULATION/ENTROPY_STAGE_INFO";
size_t DfxEntropyStageHandler::ID_GEN = 1;

DfxEntropyStageHandler::DfxEntropyStageHandler() {
	_dataObject = nullptr;
}

DfxEntropyStageHandler::~DfxEntropyStageHandler() {

}

fl::Any *DfxEntropyStageHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("entropy_stage")
		.setLabel("Entropy Stage" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "stage_type", "Stage Type", DfxStageType::ENTROPY_STAGE)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "timestep", "Timestep", 0.133f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "substeps", "Substeps", 1)
		.addProperty(space, fl::type::INT, fl::role::NONE, "diffusion_iterations", "Diffusion Iterations", 15)
		.addProperty(space, fl::type::INT, fl::role::NONE, "projection_iterations", "Projection Iterations", 30)

		.addProperty(space, fl::type::INT, fl::role::NONE, "advection_type", "Advection Type", 1)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_vorticity", "Use Vorticity", true)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "viscosity", "Viscosity", 0.05f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "vorticity", "Vorticity", 2.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "density_dissipation", "Density Dissipation", 0.06f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "buoyancy_rise", "Buoyancy Rise", 0.5f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "buoyancy_fall", "Buoyancy Fall", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ambient_temperature", "Ambient Temperature", 25.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_x", "Gravity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_y", "Gravity Y", -9.86f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_z", "Gravity Z", 0.f)

		.build(space);
}

bool DfxEntropyStageHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGES");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

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

DfxEntropyStageNode *DfxEntropyStageHandler::createStageNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

		if (any != nullptr) {
			return any->rawData<DfxEntropyStageNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_node", "Scene Graph Entropy Stage Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxEntropyStageNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxEntropyStageNode *DfxEntropyStageHandler::stageNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

	if (any != nullptr) {
		return any->rawData<DfxEntropyStageNode>();
	}
	else {
		return nullptr;
	}
}

void DfxEntropyStageHandler::destroyActorsProps() {
	/*auto node = stageNode();

	for (auto actor : node->actors()) {
		DfxBulletActorHandler::deleteNode(actor);
	}

	for (auto prop : node->props()) {
		if (prop->uniqueName() == DfxBulletConstraintPropNode::getUniqueName()) {
			DfxBulletConstraintPropHandler::deleteNode(prop);
		}
	}*/
}

bool DfxEntropyStageHandler::destroy() {

	if (_dataObject != nullptr) {

		auto space = DfxSpace::getInstance();

		auto node = stageNode();
		if (node != nullptr) {
			destroyActorsProps();

			DfxQSceneManager::getInstance()->removeNode(node);
			delete node;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Entropy Stage Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

deepsg::SgNode* DfxEntropyStageHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_node");

		auto snode = new DfxEntropyStageNode;

		scenegraph_node->newData<DfxEntropyStageNode>(DfxFluentTypes::SG_STAGE_NODE, snode);

		snode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(snode, parent);
		}

		return snode;
	}

	return nullptr;
}

void DfxEntropyStageHandler::deleteNode(deepsg::SgNode *node) {
	DfxEntropyStageHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}