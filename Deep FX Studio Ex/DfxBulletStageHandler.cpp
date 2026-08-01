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

#include "DfxBulletStageHandler.h"
#include "DfxBulletStageNode.h"
#include "DfxBulletActorHandler.h"
#include "DfxBulletConstraintPropHandler.h"
#include "DfxBulletConstraintPropNode.h"

std::string DfxBulletStageHandler::model_id = "/SIMULATION/BULLET_STAGE_INFO";
size_t DfxBulletStageHandler::ID_GEN = 1;

DfxBulletStageHandler::DfxBulletStageHandler() {
	_dataObject = nullptr;
}

DfxBulletStageHandler::~DfxBulletStageHandler() {

}

fl::Any *DfxBulletStageHandler::createModel(fl::Space *space ) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("bullet_stage")
		.setLabel("Bullet Stage" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "stage_type", "Stage Type", DfxStageType::BULLET_STAGE)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "timestep", "Timestep", 0.0166f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "substeps", "Substeps", 1)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_x", "Gravity X", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_y", "Gravity Y", -9.86f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gravity_z", "Gravity Z", 0.f)

		.build(space);
}

bool DfxBulletStageHandler::create() {
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

DfxBulletStageNode *DfxBulletStageHandler::createStageNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

		if (any != nullptr) {
			return any->rawData<DfxBulletStageNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_node", "Scene Graph Bullet Stage Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxBulletStageNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxBulletStageNode *DfxBulletStageHandler::stageNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_node");

	if (any != nullptr) {
		return any->rawData<DfxBulletStageNode>();
	}
	else {
		return nullptr;
	}
}

void DfxBulletStageHandler::destroyActorsProps() {
	auto node = stageNode();

	for (auto actor : node->actors()) {
		DfxBulletActorHandler::deleteNode(actor);
	}

	for (auto prop : node->props()) {
		if (prop->uniqueName() == DfxBulletConstraintPropNode::getUniqueName()) {
			DfxBulletConstraintPropHandler::deleteNode(prop);
		}
	}
}

bool DfxBulletStageHandler::destroy() {
	
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
			DfxGlobal::console::println("Bullet Stage Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

deepsg::SgNode* DfxBulletStageHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_node");

		auto snode = new DfxBulletStageNode;
		
		for (auto actor : snode->actors()) {
			DfxBulletActorHandler::deleteNode(actor);
		}

		scenegraph_node->newData<DfxBulletStageNode>(DfxFluentTypes::SG_STAGE_NODE, snode);

		snode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(snode, parent);
		}

		return snode;
	}

	return nullptr;

}

void DfxBulletStageHandler::deleteNode(deepsg::SgNode *node) {
	DfxBulletStageHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}