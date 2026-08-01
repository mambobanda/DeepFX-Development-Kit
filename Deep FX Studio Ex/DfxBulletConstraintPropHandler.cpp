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

#include "DfxBulletConstraintPropHandler.h"
#include "DfxBulletConstraintPropNode.h"
#include "DfxBulletConstraintWidget.h"

std::string DfxBulletConstraintPropHandler::model_id = "/SIMULATION/BULLET_CONSTRAINT_INFO";
size_t DfxBulletConstraintPropHandler::ID_GEN = 1;

DfxBulletConstraintPropHandler::DfxBulletConstraintPropHandler() {
	_dataObject = nullptr;
}

DfxBulletConstraintPropHandler::~DfxBulletConstraintPropHandler() {


}

fl::Any *DfxBulletConstraintPropHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return fl::DataObject::Builder()
		.setName("bullet_constraint_prop")
		.setLabel("Bullet Constraint Prop" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "constraint_type", "Constraint Type", (unsigned int)0)

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "body_a", "Point To Point Body A", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "body_b", "Point To Point Body B", std::string(""))

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "point_to_point_pivot", "Point To Point Pivot", std::string(""))

		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "hinge_pivot", "Hinge Pivot", std::string(""))
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "hinge_enable_motor", "Hinge Enable Motor", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "hinge_target_velocity", "Hinge Target Velocity", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "hinge_max_impulse", "Hinge Max Impulse", 2.f)
		.build(space);
}

bool DfxBulletConstraintPropHandler::create() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/STAGE_PROPS");

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

bool DfxBulletConstraintPropHandler::destroy() {

	if (_dataObject != nullptr) {

		auto space = DfxSpace::getInstance();

		auto node = propNode();
		if (node != nullptr) {
			((DfxSimulationStageBase*)node->stage())->removeProp(node->getID());
			delete node;

			auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Bullet Constraint Prop Handler: Destroy failed to clear node.");
		}

		space->remove(_dataObject);

		return true;
	}

	return false;
}

DfxBulletConstraintPropNode *DfxBulletConstraintPropHandler::createPropNode(deepsg::SgStageNode *stage) {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

		if (any != nullptr) {
			return any->rawData<DfxBulletConstraintPropNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_stage_prop_node", "Scene Graph Bullet Stage Prop Node");
			space->move(scenegraph_node, _dataObject);

			return (DfxBulletConstraintPropNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxBulletConstraintPropNode *DfxBulletConstraintPropHandler::propNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_stage_prop_node");

	if (any != nullptr) {
		return any->rawData<DfxBulletConstraintPropNode>();
	}
	else {
		return nullptr;
	}
}

deepsg::SgNode* DfxBulletConstraintPropHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_stage_prop_node");

		auto snode = new DfxBulletConstraintPropNode(nullptr);

		scenegraph_node->newData<DfxBulletConstraintPropNode>(DfxFluentTypes::SG_STAGE_PROP_NODE, snode);

		snode->setReferencePath(referencePath);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(snode, parent);
		}

		return snode;
	}

	return nullptr;
}

void DfxBulletConstraintPropHandler::deleteNode(deepsg::SgNode *node) {
	DfxBulletConstraintPropHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}