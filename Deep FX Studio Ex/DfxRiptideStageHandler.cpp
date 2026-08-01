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

#include "DfxRiptideStageHandler.h"
#include "DfxRiptideStageNode.h"

std::string DfxRiptideStageHandler::model_id = "/SIMULATION/RIPTIDE_INFO";
size_t DfxRiptideStageHandler::ID_GEN = 1;

DfxRiptideStageHandler::DfxRiptideStageHandler() {

}

DfxRiptideStageHandler::~DfxRiptideStageHandler() {

}

fl::Any *DfxRiptideStageHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("riptide_stage")
		.setLabel("Riptide Stage" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)

		/// 
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "stage_type", "Stage Type", DfxStageType::RIPTIDE_STAGE)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "device", "Device", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "resolution", "Resolution", 2.0f)

		// Search Type
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "search", "Search", 1)

		/// Timestep
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "timing_type", "Timing Type", 1)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fixed_timstep", "fixed_timestep", 0.001f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "fixed_substeps", "Fixed Substeps", 6)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "fps_timestep", "Fps Timesteps", 0.0025f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "fps_fps", "Fps Fps", 30)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "adaptive_min_timestep", "Adaptive Minimum Timestep", 0.001f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "adaptive_max_timestep", "Adaptive Maximum Timestep", 0.005f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "adaptive_fps", "Adaptive Fps", 30)

		.addProperty(space, fl::type::BOOL, fl::role::NONE,  "divergence_free", "Divergence Free", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "divergence_error", "Divergence Error", 0.1f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "density_error", "Density Error", 0.01f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "pressure_relaxation", "Pressure Relaxation", 0.50f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "coupling_stiffness", "Coupling Stiffness", 0.05f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "velocity_dampening", "Velocity Dampening", 0.002f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_x", "Gravity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_y", "Gravity Y", -9.81f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_z", "Gravity Z", 0.f)

		.build(space);
}

bool DfxRiptideStageHandler::create() {
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

	return false;
}

void DfxRiptideStageHandler::destroyActorsProps() {
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


bool DfxRiptideStageHandler::destroy() {
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
			DfxGlobal::console::println("Riptide Stage Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxRiptideStageNode *DfxRiptideStageHandler::createStageNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

		if (any != nullptr) {
			return any->rawData<DfxRiptideStageNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_node", "Scene Graph Riptide Stage Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxRiptideStageNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxRiptideStageNode *DfxRiptideStageHandler::stageNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

	if (any != nullptr) {
		return any->rawData<DfxRiptideStageNode>();
	}
	else {
		return nullptr;
	}
}

deepsg::SgNode* DfxRiptideStageHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_node");

		auto snode = new DfxRiptideStageNode;

		scenegraph_node->newData<DfxRiptideStageNode>(DfxFluentTypes::SG_STAGE_NODE, snode);

		snode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(snode, parent);
		}

		return snode;
	}

	return nullptr;
}

void DfxRiptideStageHandler::deleteNode(deepsg::SgNode *node) {
	DfxRiptideStageHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}